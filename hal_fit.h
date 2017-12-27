#ifndef HALF_FIT_H_
#define HALF_FIT_H_

/*
 * Author names:
 *   1.  uWaterloo User ID:  xxxxxxxx@uwaterloo.ca
 *   2.  uWaterloo User ID:  xxxxxxxx@uwaterloo.ca
 */

#define smlst_blk                       5 
#define smlst_blk_sz  ( 1 << smlst_blk ) 
#define lrgst_blk                       15 
#define lrgst_blk_sz    ( 1 << lrgst_blk ) 


typedef struct header {
	unsigned int prev_block :10;
	unsigned int next_block :10;
	unsigned int size :10;
	unsigned int is_alloc :1;
    unsigned int is_first :1;
}header_t;

typedef struct bucket_header {
	unsigned int prev_b_bucket :10;
	unsigned int next_b_bucket :10;
}bucket_header_t;

int find_bucket(int);
int find_appr_bucket(int);
void  half_init( void );
void split_bucket(header_t*, int);
void set_header(void*, int, int, int, int);
void set_bucket_header(void*, int, int );
void *half_alloc( unsigned int );
void* index_address(unsigned int);
unsigned int return_index( header_t *);
void toggle_bit(unsigned int);
void  half_free( void * );
void add_to_bucket (header_t *);
void *coalesce_merge(header_t*);
void remove_from_bucket( header_t* , int ); 
int check_null_next(header_t* );
int check_null_prev(header_t* );

int find_bucket_alloc(int);


#endif