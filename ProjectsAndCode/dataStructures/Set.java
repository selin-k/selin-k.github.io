/**
* A generic set.
*/
public class Set<E> {
    
    private MyArrayList<E> array = new MyArrayList<>();

    public boolean add(E element) {
        // Adds element to the list when it does not already exist.
        // Returns true on success and false otherwise.

        if (!array.contains(element)){
            array.add(element);
            return true;
        }
        return false;
    }
    
    public String toString() {
        // Returns a string representation of this Set object.
        // I printed the entire array instead of line by line
        // since I wasn't told to do it a specific way.
        if (!array.isEmpty()) {
            System.out.println(array);
        }
        return null;
    }
    
    public void clear() {
        array.clear();
    }

    public boolean contains(E element) {
        return array.contains(element);
    }

    public boolean isEmpty() {
        return array.isEmpty();
    }

    public boolean remove(E element) {
        return array.remove(element);
    }

    public int size() {
        return array.size();
    }

    public E get(int index) {
        return array.get(index);
    }
    
}
