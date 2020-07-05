#include "half_fit.h"
#include <lpc17xx.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>
#include "uart.h"
#include "half_fit.h"

//Declare 32 KiB array
unsigned char array[32768] __attribute__ ((section(".ARM.__at_0x10000000"), zero_init));

//Declare list of bucket vector bits
unsigned short bucket_bits = 0;

//Declare array of bucket adresses
header_t* bucket_ptrs[11];


header_t *MEM = (header_t *)(&array[0]);


void  half_init(void){
  uint8_t i =0;
  header_t *first_header = (header_t*)(&array[0]);
  bucket_header_t *bucket_b_header = (bucket_header_t *)(&array[4]);
  
	first_header->prev_block = 0;
	first_header->next_block = 0;
	first_header->size = 1023;
	first_header->is_alloc = 0;
  first_header->is_first = 1;
  
  bucket_b_header->prev_b_bucket = 0;
  bucket_b_header->next_b_bucket = 0;
  
  for(i; i<11; i++){
    bucket_ptrs[i] = NULL;
	}
  bucket_ptrs[10] = MEM;
  bucket_bits = 1;
  return;
}

uint32_t return_index( header_t *loc_ptr){
  uint32_t index = (uint32_t)(loc_ptr) - (uint32_t)(MEM);
	index = index/32;
  return index;
}

void* index_address(unsigned int index){
  void *ret_ptr = (void*)(MEM + index*8);
  return ret_ptr;
}

void toggle_bit(unsigned int num){
  bucket_bits ^= 1<<(10-num);
  return;
}

//Header pointer to set next and prev for buckets
void set_header(void* ptr_head, int prev, int next, int size, int alloc){
  header_t *ptr_header = (header_t *)(ptr_head);
	ptr_header->prev_block = prev ;
	ptr_header->next_block = next;
	ptr_header->size = size;
	ptr_header->is_alloc = alloc;
  ptr_header->is_first = 0;
}

void set_bucket_header(void* ptr_b_head, int prev, int next){
  bucket_header_t *ptr_b_header = (bucket_header_t *)(ptr_b_head);
  ptr_b_header->prev_b_bucket = prev;
  ptr_b_header->next_b_bucket = next;
}


//Returns pointer to first bloc in bucket of correct size.
//Size is in bytes
int find_bucket(int size){
  uint8_t m_bucket;
  uint32_t result;
  //Find minimum bucket number that would fulfill size request
  result =((size+35)/32);
  m_bucket = 31 - __builtin_clz(result);
  //m_bucket = floor(log(result)/log(2));
  return m_bucket;
}

int find_bucket_alloc(int size){
  uint8_t m_bucket;
  uint8_t i = 0;
  uint32_t result;
  //Find minimum bucket number that would fulfill size request
  result =((size+35)/32);
  m_bucket = 32 - __builtin_clz(result);
  if (1<<(m_bucket-1) == result)
      m_bucket --;
  //m_bucket = ceil(log(result)/log(2));
  
  return m_bucket;
}
  
int find_appr_bucket(int m_bucket){
  //Find a bucket of correct size with unallocated blocs
  while(1){
    //Shift so the bucket desired is leftmost bit. AND with 1 and check if empty.
    if (((bucket_bits >> (10-m_bucket)) & 1) == 1)
      break;
    else{
      m_bucket ++;
      if (m_bucket >=11)
        return -1; //to be confirmed.
    }
  }
  return m_bucket;
}
//Assigning new block with its own new header. calling set_header general fct.
//Size = How many chunks are needed for the request.
void split_bucket(header_t* buck_ptr, int size){
    int new_size;
    header_t* split_ptr;
    header_t* far_block;
    int bucket_num;
    bucket_header_t *b_header;
    bucket_header_t *first_block;
  
    new_size= (buck_ptr->size) - size;
    if((buck_ptr->size + 1)>size){
        split_ptr = buck_ptr + (size)*8; //It's incrementing in 4 bytes so 32/4 = 8
        if (buck_ptr->next_block == return_index(buck_ptr))
          set_header(split_ptr, return_index(buck_ptr), (return_index(split_ptr)), new_size,0);
        else{
          set_header(split_ptr, return_index(buck_ptr), (buck_ptr->next_block), new_size,0);
          far_block = index_address(buck_ptr->next_block);
          far_block->prev_block = return_index(split_ptr);
        }
        buck_ptr->size = size -1;
        buck_ptr->next_block = return_index(split_ptr);
   
        //Add split bloc into appropriate bucket
        bucket_num = find_bucket(((new_size+1)*32)-4);
        b_header = (bucket_header_t *)((split_ptr)+1);
        if (bucket_ptrs[bucket_num] == NULL){
            set_bucket_header(b_header, return_index(split_ptr), return_index(split_ptr));
            bucket_ptrs[bucket_num] = split_ptr;
            toggle_bit(bucket_num);
         }
        else{
           set_bucket_header(b_header, return_index(split_ptr), return_index(bucket_ptrs[bucket_num]));
           first_block = (bucket_header_t *)(bucket_ptrs[bucket_num]+1);
           first_block -> prev_b_bucket = return_index(split_ptr);
           bucket_ptrs[bucket_num] = split_ptr;
        }
    }
}

void *half_alloc(unsigned int size){
  int bucket_num = 0;
  header_t* return_ptr;
  bucket_header_t* next_bucket;
  header_t* buck_ptr;
  bucket_header_t * blk;
  
  //Routine returns
  if (size > 32764)
    return NULL;
  if(bucket_bits == 0)
    return NULL;
  
  bucket_num = find_bucket_alloc(size);
  bucket_num = find_appr_bucket(bucket_num);
	if (bucket_num == -1) return NULL;
  buck_ptr = bucket_ptrs[bucket_num];
  blk = (bucket_header_t *)(bucket_ptrs[bucket_num]+1);

  if ((blk -> next_b_bucket) == return_index(buck_ptr)){
    bucket_ptrs[bucket_num] = NULL;
    toggle_bit(bucket_num);
  }
  else
    bucket_ptrs[bucket_num] = index_address(blk->next_b_bucket); 
  

  split_bucket(buck_ptr, ((size+35)/32));
	buck_ptr->is_alloc = 1;
  return_ptr = (header_t *)buck_ptr + 1; // returning pointer to the user for insertion
  return return_ptr;
}


void add_to_bucket (header_t* temp){
    int bucket_num;
    bucket_header_t *b_header;
    bucket_header_t *first_block;

  
    bucket_num = find_bucket(((temp->size+1)*32)-4);
    b_header = (bucket_header_t *)(temp+1);
	
	if (bucket_ptrs[bucket_num] == NULL){
            set_bucket_header(b_header, return_index(temp), return_index(temp));
            bucket_ptrs[bucket_num] = temp;
            toggle_bit(bucket_num);
         }
  else{
           set_bucket_header(b_header, return_index(temp), return_index(bucket_ptrs[bucket_num]));
           first_block = (bucket_header_t *)(bucket_ptrs[bucket_num]+1);
           first_block -> prev_b_bucket = return_index(temp);
           bucket_ptrs[bucket_num] = temp;
        }
}


void *coalesce_merge(header_t* ptr_free){ 
    
    header_t* next_block_free = index_address(ptr_free->next_block);
    header_t* prev_block_free = index_address(ptr_free->prev_block);
		header_t* far_block;
	
    if( next_block_free->is_alloc == 0){
        ptr_free->size += next_block_free->size +1; //Size of 0 is 32 bytes so must add 1 if adding two sizes.
        ptr_free->next_block = next_block_free-> next_block;
				far_block = index_address(ptr_free->next_block);
        far_block->prev_block = return_index(ptr_free);
        next_block_free->is_alloc = 1;
        return ptr_free;
    }

    else if (prev_block_free->is_alloc == 0){
        prev_block_free->size += ptr_free->size +1;
        if (ptr_free->next_block == return_index(ptr_free))
          prev_block_free->next_block = return_index(prev_block_free);
        else{
          prev_block_free->next_block = ptr_free-> next_block;
          far_block = index_address(ptr_free->next_block);
          far_block->prev_block = return_index(prev_block_free);
        }
        return prev_block_free;
    }
   
        }


void remove_from_bucket( header_t* ptr_temp, int bucket_num ){
//    header_t* temp;
    bucket_header_t* remove_ptr = (bucket_header_t* )(ptr_temp + 1);
   unsigned int index = return_index( ptr_temp);
      //Case 1: next and prev are pointing to itself... only one in the list
    if( remove_ptr->next_b_bucket == index && remove_ptr->prev_b_bucket == index){
        bucket_ptrs[bucket_num] = NULL;
				toggle_bit(bucket_num);
        return; 
    }
    //Case 2: next does not point to itself , and right points to something else.... first in the list
    else if( remove_ptr->next_b_bucket != index && remove_ptr->prev_b_bucket == index){
				header_t *next_temp_blk = index_address(remove_ptr->next_b_bucket);
				bucket_header_t* next_temp = (bucket_header_t *)(next_temp_blk + 1);
        next_temp-> prev_b_bucket = return_index(next_temp_blk);
        bucket_ptrs[bucket_num] = next_temp_blk;
        return;
    }
    // Case 3: prev does not point to itslef, and right points to itself.... last in the list
    else if( remove_ptr->prev_b_bucket != index && remove_ptr->next_b_bucket == index){
        header_t *prev_temp_blk = index_address(remove_ptr->prev_b_bucket);
				bucket_header_t* prev_temp = (bucket_header_t *)(prev_temp_blk + 1);
        prev_temp->next_b_bucket = return_index(prev_temp_blk);
        return;
    }
    //Case 4: next and prev do not point to itself
    else if(remove_ptr->prev_b_bucket != index && remove_ptr->next_b_bucket != index){
				header_t *next_temp_blk = index_address(remove_ptr->next_b_bucket);
				bucket_header_t* next_temp = (bucket_header_t *)(next_temp_blk + 1);
				header_t *prev_temp_blk = index_address(remove_ptr->prev_b_bucket);
				bucket_header_t* prev_temp = (bucket_header_t *)(prev_temp_blk + 1);
        prev_temp-> next_b_bucket = remove_ptr-> next_b_bucket;
        next_temp-> prev_b_bucket = return_index(prev_temp_blk);
        return;
    }
}

void  half_free(void * address){
    header_t* temp_ptr = (header_t *)(address) - 1;
    header_t* temp_next = index_address(temp_ptr->next_block);
    header_t* temp_prev = index_address(temp_ptr->prev_block);
		int bucket_num;
	
		if (return_index(temp_ptr)>=1024) return;
		if (temp_ptr->is_alloc == 0) return; 
    bucket_num = find_bucket(((temp_next->size + 1)*32)-4);
    
    if(temp_next->is_alloc == 0){
        remove_from_bucket(index_address(temp_ptr->next_block), bucket_num);
        temp_ptr = coalesce_merge(temp_ptr);
    }
    
		bucket_num = find_bucket(((temp_prev->size + 1)*32)-4);
    if(temp_prev->is_alloc == 0){
        remove_from_bucket (index_address(temp_ptr->prev_block), bucket_num);
        temp_ptr = coalesce_merge(temp_ptr);
    }
    
		temp_ptr->is_alloc =0;
    add_to_bucket(temp_ptr);
    return;
}
