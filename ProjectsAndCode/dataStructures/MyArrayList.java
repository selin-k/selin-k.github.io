
import java.util.Arrays;
import java.util.Iterator;


public class MyArrayList<E> implements Iterable<E>{

    private Object[] array;
    private int size;
    private int capacity;

    public MyArrayList() {
        // Initialise variables
        this.capacity = 1000;
        this.array = new Object[capacity];
        this.size = 0;
    }

    public MyArrayList(int size) {
        // Initialise variables
        this.capacity = size;
        this.array = new Object[size];
        this.size = size;
    }


    public boolean add(E element) {
        // Adds element to the array, returns true on success and false otherwise.
        // Doubles the array size when reached capacity
        try {
            if (this.size >= this.capacity) {
                this.capacity *= 2;
                Object[] temp = new Object[capacity];
                System.arraycopy(this.array, 0, temp, 0, this.size);
                this.array = temp;
            }

            array[this.size++] = element;

            return true;

        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }
    }

    public boolean contains(E element) {
        // Returns true when element is in the array, false otherwise.
        for (int i = 0; i < this.size(); i++) {
            if (element.equals(this.get(i))) {
                return true;
            }
        }
        return false;
    }

    public void clear() {
        // Creates a new array and sets it to that
        this.capacity = 1000;
        this.array = new Object[capacity];
        this.size = 0;
    }

    public boolean isEmpty() {
        // Returns true if empty, false if not.
        return this.size() == 0;
    }

    public int size() {
        // Returns the size.
        return this.size;
    }

    // This line allows us to cast our object to type (E) without any warnings.
    // For further details, please see:
    // https://docs.oracle.com/javase/8/docs/api/java/lang/SuppressWarnings.html
    @SuppressWarnings("unchecked")
    public E get(int index) {
        // Returns the element from the given index in the array.
        return (E) this.array[index];
    }

    public int indexOf(E element) {
        // Returns the index if element exists in the array, -1 if does not exist.
        for (int i = 0; i < this.size(); i++) {
            if (element.equals(this.get(i))) {
                return i;
            }
        }
        return -1;
    }

    public boolean remove(E element) {
        // Returns true if element removed from array, false if not.
        // Shifts all elements down if removed
        int index = this.indexOf(element);
        if (index >= 0) {
            for (int i = index + 1; i < this.size; i++) {
                this.set(i - 1, this.get(i));
            }
            this.array[this.size - 1] = null;
            this.size--;
            return true;
        }
        return false;
    }

    public E set(int index, E element) {
        // Returns the element from the given index
        // And replaces that element at the given index with given element
        if (index >= this.size) {
            throw new ArrayIndexOutOfBoundsException("index > size: " + index + " >= " + this.size);
        }
        E replaced = this.get(index);
        this.array[index] = element;
        return replaced;
    }

    public String toString() {
        // Returns a String representation of the elements inside the array.
        if (this.isEmpty()) {
            return "Empty";
        }

        StringBuilder ret = new StringBuilder("");
        for (int i = 0; i < this.size; i++) {
            ret.append("Index: " + i + "    Element: " + this.get(i) + "\n");
        }

        ret.deleteCharAt(ret.length() - 1);

        return ret.toString();
    }

    // https://hg.openjdk.java.net/jdk8/jdk8/jdk/file/tip/src/share/classes/java/util/ArrayList.java
    @SuppressWarnings("unchecked")
    public E[] toArray(E[] a) {
        if (a.length < size)
            return (E[]) Arrays.copyOf(this.array, this.size, a.getClass());

        System.arraycopy(this.array, 0, a, 0, this.size);
        if (a.length > this.size())
            a[this.size] = null;
        return a;
    }


    /** Returns an instance of the Iterator class below. */
    @Override
    public Iterator<E> iterator() {
        return new ArrayListIterator();
    }

    /** Allows Arraylists to be iterated with 
     * for each loop syntax or iterator objects.
     */
    private class ArrayListIterator implements java.util.Iterator<E> {

        private int current = 0; /** Starts from the first element */

        /** Checks if there is another element 
         * after the current index of the iterator.
         * @return true if there is, false otherwise.
         */
        public boolean hasNext() {
            return current < size();
        }

        /**
         * Returns the next element in the arraylist
         * if it has one.
         * @return the next element in the arraylist
         * if it exists, otherwise throws an exception.
         */
        @SuppressWarnings("unchecked")
        public E next() {
            if (!hasNext()) throw new java.util.NoSuchElementException();
            return (E) array[current++];
        }

        /**
         * Removes the current element that the iterator
         * is at.
         */
        @SuppressWarnings("unchecked")
        public void remove() {
            MyArrayList.this.remove((E)array[--current]); // reference the outer class
        }
    }



}

