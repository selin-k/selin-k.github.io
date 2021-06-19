

/**
 * Implements a HashMap with linear probing.
 * References from <a href="https://github.com/GodTamIt"></a>
 * 
 * @param <K> type of keys.
 * @param <V> type of values.
 * @author Selin Kayay
 * @version 1.0
 */
public class MyHashMap<K, V> {

    private MapEntry<K, V>[] table;            /** The table to contain the buckets. */
    private int size;                          /** Size of the table. */
    private int STARTING_SIZE = 10;            /** Default starting size. */
    private double MAX_LOAD_FACTOR = 0.67;     /** Consant starting load factor. */

    /** Constructor initalizes a map entry array of default starting
     * size and sets current size to 0.
     */
    @SuppressWarnings("unchecked")
    public MyHashMap() {
        this.table = new MapEntry[STARTING_SIZE];
        this.size = 0;
    }


    /**
     * Adds the given key-value pair to the HashMap.
     * If the key is already in the HashMap, the value is overwritten.
     * 
     * @param key is the key to inserted.
     * @param value is the value to be inserted.
     * @throws IllegalArgumentException if key or value is null.
     * @return null if the key was not already in the map.  If it was in the
     * map, return the old value associated with it.
     */
    public V put(K key, V value) {

        /** Avoid inserting null keys or values into the map. */
        if (key == null || value == null) {
            throw new IllegalArgumentException("Argument cannot be null.");
        }

         /** Resize the table before adding if the current
         * load factor (the current size + 1 / table size)
         * exceeds the constant load factor. Generally the 
         * table will be resized when it is a single element
         * greater than the current size.
        */
        if (this.getNextLoadFactor() > MAX_LOAD_FACTOR) {
            resize();
        }

        /** Get the index to insert to given the key to put 
         * to the HashMap, then get the entry at this index.
         */
        int index = this.getAddIndex(key);
        MapEntry<K, V> entry = this.table[index];
        V result = null;

        if (entry == null) {
            /** If entry is null create a new entry there and
             * insert the key value pair.
             */
            this.table[index] = new MapEntry<K, V>(key, value);
        } else if (entry.getKey().equals(key)) {
            /** Duplicate key found then get the current 
             * value of that entry and then set the given
             * value instead. The previous value will be 
             * returned.
             */

            if (!entry.isRemoved()) {
                result = entry.getValue();
            }
            entry.setValue(value);
            entry.setRemoved(false);
        } else {
            /** Set the key and value given that 
             * an entry exists but the key is not 
             * a duplicate.
             */

            entry.setKey(key);
            entry.setValue(value);
            entry.setRemoved(false);
        }

        /** If result is null this means a new
         * entry was made hence size is incremented.
         */

        if (result == null) {
            size++;
        }

        return result;
    }


    /**
     * Removes the value associated with the key from the map.
     * 
     * @param key is the key to remove.
     * @throws IllegalArgumentException if key is null.
     * @return null if the key was not already in the map.  If it was in the
     * map, return the old value associated with it.
     */
    public V remove(K key) {
        if (key == null) {
            throw new IllegalArgumentException("Argument cannot be null.");
        }

        /** If key is not null get its index and get the 
         * entry from the table.
         */
        int index = this.getAddIndex(key);
        MapEntry<K, V> entry = this.table[index];

        /** If the entry indeed has the given key within the map 
         * remove the entry, decrement the size of the table,
         * and return the value of the removed entry.
         */
        if (entry != null && entry.getKey().equals(key) && !entry.isRemoved()) {
            entry.setRemoved(true);
            size--;
            return entry.getValue();
        }

        return null;
    }


 /**
     * Gets the value associated with the given key.

     * @param key is the key to be looked up.
     * @throws IllegalArgumentException if key is null.
     * @return the value associated with the given key.
     */
    public V get(K key) {
        if (key == null) {
            throw new IllegalArgumentException("Argument cannot be null.");
        }

        /** Get the entry within the map with the given key. */
        int index = this.getAddIndex(key);
        MapEntry<K, V> entry = this.table[index];

        /** return the value of the key pair. */
        return (entry != null && !entry.isRemoved()
                && entry.getKey().equals(key) ? entry.getValue() : null);
    }


    /**
     * Returns whether or not the key is in the map.

     * @param key is the key to be looked up.
     * @throws IllegalArgumentException if key is null
     * @return true if the key is in the map, false otherwise.
     */
    public boolean containsKey(K key) {
        return get(key) != null;
    }

    /**
     * @return number of elements in the HashMap.
     */
    public int size() {
        return this.size;
    }

    
    /**
     * @return the keys within the HashMap as an ArrayList.
     */
    public MyArrayList<K> keyList() {
        MyArrayList<K> result = new MyArrayList<>();

        /** Traverse the table and for each existing 
         * entry if it is not null add the associated
         * key into the arraylist "result".
         */
        for (int i = 0; i < this.table.length; i++) {
            MapEntry<K, V> entry = this.table[i];
            if (entry != null && !entry.isRemoved()) {
                result.add(entry.getKey());
            }
        }

        return result;
    }

    /**
     * @return an Arraylist of all the values within the HashMap.
     */
    public MyArrayList<V> values() {
        MyArrayList<V> result = new MyArrayList<>();


        /** Traverse the map and add the value of each entry
         * given it is not null.
         */
        for (int i = 0; i < this.table.length; i++) {
            MapEntry<K, V> entry = this.table[i];
            if (entry != null && !entry.isRemoved()) {
                result.add(entry.getValue());
            }
        }
        return result;
    }

    /** PRIVATE HELPERS */

    /**
     * Calculates the load factor if another element is added the HashMap.
     *
     * @return Returns the load factor.
     */
    private double getNextLoadFactor() {
        return (double) (size + 1) / (double) this.table.length;
    }

    /**
     * Doubles the size of the HashMap and copies the current HashMap
     * into it.
     */
    @SuppressWarnings("unchecked")
    private void resize() {
        /** Keep a copy of the current table and then create a 
         * new one of double the size.
         */
        MapEntry<K, V>[] temp = this.table;
        this.table = (MapEntry<K, V>[]) new MapEntry[temp.length * 2];
        this.size = 0;

        /** Traverse the previous table and place all 
         * entries into the new instance given not null.
         */
        for (int i = 0; i < temp.length; i++) {
            MapEntry<K, V> entry = temp[i];
            if (entry != null && !entry.isRemoved()) {
                this.put(entry.getKey(), entry.getValue());
            }
        }

    }

    /**
     * Given the key to add gets an index to add to.
     * 
     * @param key the key to be added.
     * @return the index to add the data into.
     */
    private int getAddIndex(K key) {
        int index = Math.abs(key.hashCode()) % this.table.length;

        MapEntry<K, V> curEntry = this.table[index];
        int firstRemoved = -1;
        int curIndex = index;
        boolean looped = false;

        while (curEntry != null && !curEntry.getKey().equals(key)
                && (!looped || curIndex != index)) {
            if (firstRemoved == -1 && curEntry.isRemoved()) {
                // Record first removed
                firstRemoved = index;
            }

            if (++curIndex >= this.table.length) {
                // Loop back around to the first of the table
                curIndex = 0;
                looped = true;
            }

            curEntry = this.table[curIndex];
        }
        if (curEntry != null && curEntry.getKey().equals(key)) {
            return curIndex;
        } else {
            return (firstRemoved == -1 ? curIndex : firstRemoved);
        }
    }
}

/**
 * Map entries are some key value pairs.
 */
class MapEntry<K, V> {
    private boolean removed;
    private K key;
    private V value;

    /** Constructor given some key value pair. */
    public MapEntry(K k, V v) {
        key = k;
        value = v;
    }

    public boolean isRemoved() {
        return removed;
    }

    /** Getters and setters */
    public void setRemoved(boolean removed) {
        this.removed = removed;
    }

    public K getKey() {
        return key;
    }

    public void setKey(K key) {
        this.key = key;
    }

    public V getValue() {
        return value;
    }

    public void setValue(V value) {
        this.value = value;
    }

    public boolean equals(Object o) {
        if (!(o instanceof MapEntry)) {
            return false;
        } else {
            @SuppressWarnings("unchecked")
            MapEntry<K, V> that = (MapEntry<K, V>) o;
            return that.getKey().equals(key) && that.getValue().equals(value);
        }
    }

    public String toString() {
        return key.toString() + ": " + value.toString();
    }
}
