public class DoublyLinkedList<E> {

    private ListElement<E> head;
    private ListElement<E> tail;

    public DoublyLinkedList() {
        head = null;
        tail = null;
    }

    int count = 0;

    public void addToHead(E value) {
        ListElement<E> e = new ListElement<>(value);

        if (!isEmpty()) {
            e.setNext(head);
            head.setPrev(e);
        } else { // empty
            tail = e;
        }

        head = e;
        count++;
    }

    public void addToTail(E value) {
        ListElement<E> e = new ListElement<>(value);

        if (!isEmpty()) {
            tail.setNext(e);
            e.setPrev(tail);
        } else { // empty
            head = e;
        }

        tail = e;
        count++;
    }

    public E removeFromHead() {
        if (isEmpty()) {
            return null;
        }

        ListElement<E> e = head;

        head = head.getNext();

        if (isEmpty()) {
            tail = null;
        } else {
            head.setPrev(null); // the first element has no predecessors
        }
        count--;
        return e.getValue();
    }

    public E removeFromTail() {
        if (isEmpty()) {
            return null;
        }

        ListElement<E> e = tail;

        tail = tail.getPrev();

        if (isEmpty()) {
            head = null;
        } else {
            tail.setNext(null); // the last element has no successors
        }
        count--;
        return e.getValue();
    }

    public boolean isEmpty() {
        return (head == null) || (tail == null);
    }

    public int size() {
        return count;
    }

    public void clear() {
        count = 0;
        head=null;
        tail=null;
    }


}


class ListElement<E> {
    private final E value;
    private ListElement<E> next;
    private ListElement<E> prev;
    
    public ListElement(E value) {
        this.value = value;
    }
    
    public E getValue() {
        return this.value;
    }
    
    public ListElement<E> getNext() {
        return this.next;
    }
    
    public ListElement<E> getPrev() {
        return this.prev;
    }
    
    public void setNext(ListElement<E> e) {
        this.next = e;
    }
    
    public void setPrev(ListElement<E> e) {
        this.prev = e;
    }

}
