
import java.util.Comparator;

/**
 * Represents the nodes that can be 
 * stored in the AVL tree.
 */
class Node<T>
{
  T data;
  Node<T> left, right;
  int height;
 
  Node(T data)
  {
    this.data = data;
    this.height = 1;
  }
}

/**
 * A generic self-balancing BST (AVL tree). Self-balancing algorithm
 * references from here: <a href="https://www.geeksforgeeks.org/"></a>
 * 
 * @author Selin Kayay
 * @version 1.0
 */
public class BST<T>
{
  private Node<T> root;               /** Root node of the tree. */
  public int size;                    /** Keeps track of number of nodes in tree. */
  private Comparator<T> comparator;   /** The comparator instance in the public 
                                      methods */

  /** Constructors */

  @SuppressWarnings("unchecked")
  public BST() {
      this.root = null;
      this.size = 0;
      this.comparator = (Comparator<T>) Comparator.naturalOrder();
  }

  /** Construct a BST with a custom Comparator. */
  public BST(Comparator<T> comparator) {
    this.root = null;
    this.size = 0;
    this.comparator = comparator;
  }

  /**
   * Returns the number of nodes in the tree
   * in the called instance.
   * @return number of nodes in the tree.
   */
  public int size() {
    return size;
  }

  /**
   * Clear a tree instance.
   */
  public void clear() {
    this.root = null;
  }


  public static int flag = 0; /** 0 if no duplicate node found during insertion, 1 otherwise. */
 
   /**
   * Wrapper for the recursive overloaded method `insert` below.
   * This method adds a node to a tree instance.
   * @param data is the generic data to be added.
   * @return true if added, false otherwise.
   */
  public boolean insert(T data)
  {
    size++;
    Node<T> temp = insert(root, data);

    /** Flag will be set to 1 within the helper
     * method when a duplicate node was attempted
     * to be inserted. This logic is useful for 
     * easier implementations of addX within the 
     * stores.
     */
    if ( flag == 1 ) {
      flag = 0;
      root = temp;
      return false;
    }

    root = temp;
    return true;
  }
 
  /** Helper for insert, traverses the tree as usual starting from root
   * to find the location which to where the node should be inserted.
   * When location is found the node is inserted the tree is rebalanced
   * if required.
   * 
   * @param root is the starting, root node.
   * @param data is the node to be inserted.
   * @return the root node after operation.
   * 
   */
  private Node<T> insert(Node<T> root, T data)
  {
    /** instantiate a new Node with data as data
     * if the current root has not been previously
     * instantiated */
    if (root == null) return new Node<T>(data);
  
    /** If the value of the data being searched for is
     * less than the value of the current root node, then
     * traverse to the left node of the current root, setting
     * the current left node to whatever gets returned from
     * the insert method. 
    */
    else if (comparator.compare(data,root.data) < 0)
      root.left = insert(root.left, data);
  
    /** if the value of the data being searched for is less
     * than the value of the current root node, then traverse
     * to the right node of the current root, setting the current
     * right node to whatever gets returned from the insert method.
    */
    else if (comparator.compare(data, root.data) > 0)
      root.right = insert(root.right, data);

    else
    {
      /** This means a duplicate node was
       * found in the tree. Going by the logic
       * required for the stores here we return 
       * the node after setting "flag" to 1 which
       * will end up returning "false" from insert.
       */
      flag = 1;
      size--;
      return root;
      
    }

    /** After insertion : BALANCING */

    /** Update height of this ancestor node */
    root.height = 1 + max(height(root.left), height(root.right));

    /** Get the balance factor of this ancestor
    node to check whether this node became
    unbalanced */
    int balance = getBalance(root);

    /** If this node becomes unbalanced, then there
    are 4 cases. */
    /** Left Left Case */
    if (balance > 1 && comparator.compare(data,root.left.data) < 0 )
        return rightRotate(root);

    /** Right Right Case */
    if (balance < -1 && comparator.compare(data,root.right.data) > 0 )
        return leftRotate(root);

    /** Left Right Case */
    if (balance > 1 && comparator.compare(data,root.left.data) > 0 ) {
        root.left = leftRotate(root.left);
        return rightRotate(root);
    }

    /** Right Left Case */
    if (balance < -1 && comparator.compare(data,root.right.data) < 0 ) {
        root.right = rightRotate(root.right);
        return leftRotate(root);
    }
 
    return root;
  }
 
 
  /**
   * Removes a given node from the tree if found.
   * Calls the recursive helper method `delete` which 
   * actually searches for the node and removes if found.
   * @param data is the node to be removed.
   */
  public void delete(T data)
  {
    size--;
    root = delete(root, data);
  }
 
  /**
   * Helper function for the wrapper above. Searches for some 
   * node "data" starting from "root". Removes it if found and 
   * balances the tree.
   * @param root is the starting, root node.
   * @param data is the node to be deleted.
   * @return the new root.
   */
  private Node<T> delete(Node<T> root, T data)
  {
    /** if the root node is null, then either there's nothing
     *  to delete or no more traversal is necessary
     */
    if (root == null) return root;
  
    else if (comparator.compare(data,root.data) < 0 ) 
      root.left = delete(root.left, data);

    else if (comparator.compare(data,root.data) > 0)
      root.right = delete(root.right, data);

    /** The node to delete has been found. */
    else
    {
      /** if the node has no children, then return a value of null */
      if (root.left == null && root.right == null)
        return null;
    
      /** if the node has a left child, but no right child, then
       * return the left child */
      else if (root.right == null) return root.left;
      
      /** if the node has a right child, but no left child, then
       *  return the right child */
      else if (root.left == null) return root.right;
      
      /** if the node has two children, then set the node's data to
       * be the largest element in the left sub-tree of the node,
       * and then set the left child's data to be equal to whatever
       * data is returned when deleting the new root data from the
       * left sub-tree
       */
      else
      {
        Node<T> temp = minValueNode(root.right);
        root.data = temp.data;
        root.right = delete(root.right, temp.data); 
      }
    }

    /** BALANCING  */

    if (root == null) return root;

    /** Update height of the current node. */ 
    root.height = max(height(root.left), height(root.right)) + 1;  


    /** Check if the current node has been unbalanced */ 
    int balance = getBalance(root);  

    /** Check the 4 cases and rotate accordingly to balance. */
    if (balance > 1 && getBalance(root.left) >= 0)  
        return rightRotate(root);  
 
    if (balance > 1 && getBalance(root.left) < 0)  
    {  
        root.left = leftRotate(root.left);  
        return rightRotate(root);  
    }  

    if (balance < -1 && getBalance(root.right) <= 0)  
        return leftRotate(root);  

    if (balance < -1 && getBalance(root.right) > 0)  
    {  
        root.right = rightRotate(root.right);  
        return leftRotate(root);  
    }  
 
    return root;
  }


  /**
   * Given some node which is the root of some subtree,
   * returns the minimum value node in the tree to help
   * with `delete` when some node with two children is
   * being removed.
   * @param node is the root of the subtree.
   * @return the minimum value node in the given 
   * subtree.
   */
  private Node<T> minValueNode(Node<T> node)  
  {  
    Node<T> current = node;  

    /** Loop down to find the leftmost leaf */
    while (current.left != null)  
      current = current.left;  
    return current;  
  } 
 
 
  /**
   * Returns true if the value is contained in the BST 
   * and false otherwise.
   * @param data the value that's being searched for.
   * @return true if found, false otherwise.
   * @see boolean
   */
  public boolean contains(T data)
  {
    return contains(root, data);
  }
 
  /** Recursive binary search for some node in the tree
   * starting with given root.
   * @param root is the starting root.
   * @param data is the node being searched for.
   * @return true if in tree, otherwise false.
   */
  private boolean contains(Node<T> root, T data)
  {
    /** if the root is null, then either the BST
     * is empty or the value has not been found
     * and traversal cannot continue */
    if (root == null) return false;

    /** Search  */

    else if (comparator.compare(data,root.data) < 0)
      return contains(root.left, data);

    else if (comparator.compare(data,root.data) > 0)
      return contains(root.right, data);

    /** Data found. */
    else
      return true;

  }

  /**
   * Given some node the tree is searched for a node with 
   * duplicate field for whatever the comparator instance 
   * of the tree compares for.
   * @param data is the node to get.
   * @return the node with the matching fields.
   */
  public T get(T data)
  {
    return get(root, data);
  }
 

  /**
   * The recursive helper for get. Searches the tree as usual and returns
   * the node when the comparison indicates
   * @param root is the starting root node.
   * @param data is the node to compare with the tree nodes.
   * @return the node that has a matching field.
   */
  private T get(Node<T> root, T data)
  {
    /** Base case. */
    if (root == null) return null;

    else if (comparator.compare(data,root.data) < 0)
      return get(root.left, data);

    else if (comparator.compare(data,root.data) > 0)
      return get(root.right, data);

    else
      return root.data;

  }


/** Given some node in the tree, find another node in the tree through 
 * comparator and replace that node with the given one.
 */
  public T getIf(T data, Comparator<T> comp)
  {
    /** Replace comparator instance to the 
     * given comparator "comp". Keep the 
     * initial comparator and set back before
     * returning.
     */
    Comparator<T> prevcomp = this.comparator;
    this.comparator = comp;
    T temp = getIf(root, data);
    this.comparator = prevcomp;
    return temp;
  }
 
  /**
   * Search the tree as usual and return node when found 
   * to have duplicate field to given data node.
   * @param root is the starting root node.
   * @param data is the node to be compared to.
   * @return the node in the tree with duplicate fields
   * to "data".
   */
  private T getIf(Node<T> root, T data)
  {
    if (root == null) return null;
    
    else if (comparator.compare(data,root.data) < 0)
      return getIf(root.left, data);

    else if (comparator.compare(data,root.data) > 0)
      return getIf(root.right, data);

    else
    {
      size--;
      return root.data;
    }
  }

  /** BALANCING HELPERS */

  /** 
   * Given some node return its height.
   * @param n is the node 
   * @return height of the node
   */
  private int height(Node<T> n) {
    if (n == null)
      return 0;
    return n.height;
  }

  /**
   * Given two integers return the maximum.
   * @param a is some integer
   * @param b is some integer
   * @return the maximum; either a or b.
   */
  private int max(int a, int b) {
      return (a > b) ? a : b;
  }


  /**
   * Right rotates a subtree given its root node.
   * @param y is the root node of the subtree.
   * @return the new root after rotation.
   */
  private Node<T> rightRotate(Node<T> y) {
    Node<T> x = y.left;
    Node<T> T2 = x.right;  

    /** Perform rotation */
    x.right = y;
    y.left = T2;

    /** Update heights */
    y.height = max(height(y.left), height(y.right)) + 1;
    x.height = max(height(x.left), height(x.right)) + 1;

    /** Return new root */
    return x;
  }


  /**
   * Left rotates a subtree given its root node.
   * @param x is the root node of the subtree.
   * @return the new root after rotation.
   */
  private Node<T> leftRotate(Node<T> x) {
    Node<T> y = x.right;
    Node<T> T2 = y.left;

    /** Perform rotation */
    y.left = x;
    x.right = T2;

    /** Update heights */
    x.height = max(height(x.left), height(x.right)) + 1;
    y.height = max(height(y.left), height(y.right)) + 1;

    /** Return new root */
    return y;
  }

  /**
   * Given some node, get its balance factor.
   * The balance factor is the height of the left subtree
   * minus the right.
   * @param N is the node
   * @return the balance factor, an integer value.
   */
  private int getBalance(Node<T> N) {
    if (N == null)
        return 0;

    return height(N.left) - height(N.right);
  }

  
  /** INORDER TRAVERSAL METHODS FOR TREESORT**/

  private BST<T> temptree;

  /**
   * Given some destination tree, copy the given instance
   * to the destination.
   * @param dest is the tree to be copied to.
   * @return the resulting tree.
   */
  public BST<T> copyToTree(BST<T> dest) {
    temptree = dest;
    copyToTree(root);
    return dest;
  }

  /**
   * The recursive method that inserts the 
   * current instance to "temptree" inorder.
   * @param root is the root of given instance.
   */
  private void copyToTree(Node<T> root)
  {
    if (root == null)
      return;
 
    copyToTree(root.left);
    temptree.insert(root.data);
    copyToTree(root.right);
  }


  private T[] testarray; /** Some generic array */
  private int index;     /** Index kept when inserting to array inorder. */

  /**
   * Given some generic array of set size.
   * Is filled inorder with instance of tree
   * and returned.
   * @param array is the array to be filled.
   * @return the filled array.
   */
  public T[] inorderToArray( T[] array )
	{
    testarray = array;
    this.index = 0;
		inorderToArray(root);
		return testarray;
	}

  /**
   * The recrusive method that traverses the 
   * tree instance inorder and inserts to "testarray".
   * @param root is the starting root node.
   */
	private void inorderToArray(Node<T> root)
	{
		if (root == null)
			return ;

		inorderToArray(root.left);
		testarray[index++] = root.data;
		inorderToArray(root.right);
	}


  private MyArrayList<T> array;       /** Arraylist to be returned */
  /**
   * Traverses the tree inorder and 
   * safely returns a generic array.
   * @param void
   * @return void
   */
  public T[] inorder(T[] a)
  {
    array = new MyArrayList<>();
    //System.out.println("In-order Traversal:");
    inorder(root);
    return array.toArray(a);
  }

  private void inorder(Node<T> root)
  {
    if (root == null)
      return;
 
    inorder(root.left);
    array.add(root.data);
    inorder(root.right);
   }
  

 
 
 
}
