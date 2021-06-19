
/**
 * Structure used to keep a pair of comparable key value
 * pairs.
 * Specifically used for the convertToPlace util class.
 * 
 * Overrides hashcode and equals such that instances of this 
 * class are equal objects not if they have the same hashcode
 * but if their fields are equal. This was useful to implement
 * the `convert` method efficiently.
 * 
 * @author Selin Kayay
 * @version 1.0
 */
public class KeyValuePair<K extends Comparable<K>,V> implements Comparable<KeyValuePair<K,V>> {

    protected K key;
    protected V value;
    
    /**
     * Main constructor for a key value pair object.
     * @param k is the key to be stored.
     * @param v is the value to be stored.
     */
    public KeyValuePair(K k, V v) {
        this.key = k;
        this.value = v;
    }
    
    /** Getters and Setters */

    public K getKey() {
        return key;
    }
    
    public V getValue() {
        return value;
    }

    /** Overrides */

    @Override
    public int compareTo(KeyValuePair<K,V> o) {
        return o.getKey().compareTo(this.getKey());
    }

    /** This method makes sure that when two KeyValuePair
     * objects are checked if equal that they are equal if
     * their fields are identical and it doesn't depend on
     * the objects being equal in terms of hashcode only.
     * 
     * @param o is the Object being checked against some 
     * instance of this class.
     * @return true if equal objects false otherwise.
     */
    @SuppressWarnings("unchecked")
    @Override
    public boolean equals(Object o) {
        /** Check object */
        if (this == o) return true;
        if (o == null) return false;
        if (getClass() != o.getClass()) return false;

        final KeyValuePair<K,V> temp = (KeyValuePair<K,V>) o;

        /** Compare fields */
        if (key == null) {
            if (temp.key != null)
                return false;
        } else if ( !key.equals(temp.key) )
            return false;

        if (value == null) {
            if (temp.value != null)
                return false;
        } else if (!value.equals(temp.value))
            return false;

        return true;
    }

    /** Calculates a hashcode for the KVP objects
     * instantiated.
     * @return the hashcode
     */
    @Override
    public int hashCode() {
        final int prime = 31;
        int result = 1;
        result = prime * result + ((key == null) ? 0 : key.hashCode());
        result = prime * result + ((value == null) ? 0 : value.hashCode());
        return result;

    }
}
