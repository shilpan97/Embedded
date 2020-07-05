class Node(object):
    def __init__(self, interval):
        self.i = interval
        self.left = None
        self.right = None
        self.max = None

class IntervalTree(object):

    def __init__(self,root):
        self.root = root


    def doOverlap(self, interval_left, interval_right):
        if((interval_left[0] <= interval_right[1]) and (interval_right[0] <= interval_left[1])):
            return True
        
        return False
    
    # searches a given range i in a given
    def overlapSearch(self,root, i):

        # Base Case if tree is empty
        if (root == None): return None
    
        # If given range overlaps with root
        if (self.doOverlap(root.i, i.i)):
            return root
    
        # If left child of root and max of left child >=
        # given range, then i may overlap with range in left subtree
        if (root.left != None and root.left.max > i.i[0]):
            return self.overlapSearch(root.left, i)
    
        # Else range overlaps with right subtree
        return self.overlapSearch(root.right, i)
    
    # function return all ranges in root which overlap by range i.
    def findOverlap(self,root, i , result, index):
        # Base case if reach None and check right side branch, we stop
        if (root== None):
            return result
        
        if (self.doOverlap(root.i,i) is not False):
            result.extend([root.i])
            index +=1
        
        # left child exists and may overlap
        if(root.left != None and i.i[0] < root.left.max):
            if(root.right != None and i.i[1] > root.right.i[0]):
                result = self.findOverlap(root.right,i,result,index)
            return self.findOverlap(root.left,i,result,index)
        else:
            return self.findOverlap(root.right,i,result,index)

    def isRange(self,root, i):
        if (root == None or i is None):
            return None
        
        if(   root.i[0] == i[0] and root.i[1] == i[1]):
            return True
        
        # If left child of root and max of left child >=
        # given range, then i overlap with a range in left subtree
        if (root.left != None and i[0] < root.left.max):
            return self.isRange(root.left, i)
            
        # Else range overlap with right subtree
        return self.isRange(root.right, i)    
    

    #O(log(N))
    def Get(self,root,i):
        result = [] 
        index = 0 
        
        result=self.findOverlap(root,i,result,index)
        
        if (result == None):
            print("no overlapped ranges")
        else:
            print("The Overlapped Ranges are: ")
            print(result)
        return result
    
    #O(log(N))
    def Add(self,root, i):
        if root == None:
            return Node(i.i)
        
        # findOverlap low and high values of range at root
        l = root.i[0]
        h = root.i[1]
    
        
        # check if range is subset of existing range
        if (i.i[0] >= l and i.i[1] <= h):
            return root 
        
        
        # check if  range is overlapping existing ranges
        # case 1: old range is entirely within  new range
        # check  left and right subtree to see if it overlaps those ranges
        if (i.i[0] <= l and i.i[1] >= h):
            temp = self.overlapSearch(root, i)
            temp.i[0] = i[0]
            temp.i[1] = i[1]
            root.max = i[1]
            i=temp
            
            # if left child  and it's max value > low value of the range, then it may overlap this range
            if(root.left != None and root.left.max > temp.low):
                return self.Add(root.left,i)
            
            # Else check if  overlaps the right child if that child exists
            elif (root.right != None):
                return self.Add(root.right,i)
            
            # Else return root
            else:
                return root
        
        # case 2: old range overlaps and extends over upper bound
        # check  right subtree to see if it overlaps that range
        if (i.i[0] >= l and i.i[0] < h and i.i[1] >= h):
            temp = self.overlapSearch(root,i)
            temp.i[1] = i.i[1]
            i=temp
            if (root.right != None):
                return self.Add(root.right,i)
            else:
                return self.Add(root,i)
             
        # case 3: old range overlaps and extends to lower bound
        # check  left subtree to see if it overlaps that range
        elif (i.i[0] <= l and i.i[1] > l and i.i[1] <= h):
            temp = self.overlapSearch(self,root,i)
            temp.i[0] = i[0]
            i=temp
            if (root.left != None):
                return self.Add(root.left,i)
            else:
                return self.Add(root,i)
            
        
        # If root's low value is smaller, then new range goes to left subtree
        if (i.i[0] < l):
            root.left = self.Add(root.left, i)
    
        # Else new node goes to right subtree.
        else:
            root.right = self.Add(root.right, i)
    
        return root
    

    # function when Delete only needs to adjust range values instead of delete node or split node.
    def adjnode(self,root, i):
        if (root == None):
            return # no ranges currently exist
        # Find high and low values of the range
        l =root.i[0]
        h =root.i[1]
        
        # Base case:
        # range to be deleted doesn't exist.
        # return root unchanged.
        if (self.overlapSearch(root,i) == None): 
            return 
        
        if (i.i[0] > h): 
            return self.adjnode(root.right,i)
        elif (i.i[1] < l): 
            return self.adjnode(root.left,i)
        
        # case 1:range to be deleted partially overlaps and extends over the upper bound
        elif (i.i[0] >= l and i.i[0] <= h and i.i[1] > h):
            root.i[1]=i.i[0] # change upper bound
            root.max=i.i[0] # track max value
            
            if (i.i[1] > h and root.right != None): 
                return self.adjnode(root.right,i)
            
            elif (i.i[1] >= l and i.i[1] < h): 
                root.i[0] = i.i[1] #changing lower bound
                return self.adjnode(root,i)
            
            
        # case 2:range to be deleted partially overlaps and extends below the lower bound
        elif (i.i[0] < l and i.i[1] < h and i.i[1] > l):
            root.i[0]=i.i[1] #changing lower bound
            
            if (i.i[0] < l and root.left != None):
                return self.adjnode(root.left,i)
        
            elif (i.i[0] >= l and i[0] < h): 
                    root.i[1] = i.i[0] #changing lower bound
                    return self.adjnode(root,i)
        return

    #find  node with the min value of range.
    def findmin(self,root): 
        if (root.left != None):
            return self.findmin(root.left)
        else: 
            return root
    
    def copydata(self,root, toCopy):
        root.i[0]=toCopy.i[0]
        root.i[1]=toCopy.i[1]
        root.max=toCopy.max
        return root
    
    # delete the passed node but fix all links
    def rmnode(self,root): 
        if (root.left == None and root.right == None): #no child
            del root
            root = None

        elif (root.right != None and root.left == None): #one right child
            temp = root
            root=root.right
            del temp
        
        
        elif (root.left !=None and root.right == None): #one left child
            temp = root
            root=root.left
            del temp
        
        else: #two children
        
            temp = self.findmin(root.right)#min value range node in temp
            root=copydata(root,temp)
            root.right = self.Delete(root.right,temp.i)
        
        return root

    #O(log(N))
    def Delete(self,root, i):
        if (root == None): return root
        
        # high and low values of the range
        l=root.i[0]
        h=root.i[1]
        
        # Base Case:
        if (self.overlapSearch(root,i) is None):
            print("Deleting Done")
            return root
        
        if (i.i[0] >= l and i.i[0] <= h and i.i[1] > h): #only need to adjust range values of node
            self.adjnode(root,i)
            return root
        
        elif (i.i[0] < l and i.i[1] < h and i.i[1] > l): #only need to adjust range values of node
            self.adjnode(root,i)
            return root    
        # case 1:
        # range to be deleted is contained in an existing range
        # adjust the range of  node and Add truncated part of the range.
        if (i.i[0] >= l and i.i[1] <= h):
            temp1 = root.i
            root.i[0] = i.i[1]# change the lower bound
            temp1.i[1] = i.i[0]# change the upper bound of copied range
            return self.Add(root,temp1)
        
        
        # case 2:
        # range to be deleted completely overlaps an existing range.
        else:
            # check if the range to be deleted overlaps the parent root
            if (i[0] <= l and i[1] >= h):
                root = self.rmnode(root)
                return self.Delete(root,i)
            
            if(root.left != None and i[0] < root.left.i[1]):
                root.left = self.Delete(root.left,i)
            else:
                if(root.right != None and i[1] > root.right.i[0]):
                    root.right = self.Delete(root.right,i)
        
        return root


if __name__ == '__main__':

    root = Node([1, 6])
    It = IntervalTree(root)
    root = It.Add(root, Node([3, 7]))
    root = It.Add(root, Node([8,15]))
    It.Delete(root, Node([4,10]))
    It.Get(root,[1,2])