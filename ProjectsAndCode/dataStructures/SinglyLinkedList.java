
public class SinglyLinkedList<E> {

    private Node<E> head;
    private Node<E> tail;
    private int size;


    public SinglyLinkedList() {
        this.head = null;
        this.tail = null;
        this.size = 0;
    }

    public int getSize() { 
        return this.size; 
    }


    public boolean isEmpty() { 
        return size == 0; 
    }
    
    public void addFirst(E e) {
        head = new Node(e, head);
        if ( size == 0 ) tail = head;
        size++;
    }

    public void addLast(E e) {
        Node<E> temp = new Node(e, null);
        if ( isEmpty() ) {
            head = temp;
        } else {
            tail.setNext(temp);
        }
        tail = temp;
        size++;
    }

    public E removeFirst() {
        if ( isEmpty() ) return null;
        E toReturn = head.getElement();
        head = head.getNext();
        size--;

        if ( getSize() == 0 ) tail = null;
        return toReturn;
    }

    public static void main(String[] args) {

        SinglyLinkedList<Integer> list = new SinglyLinkedList<>();

        list.addFirst(1);
        list.addLast(5);
        list.addLast(6);
        list.addLast(7);

        System.out.println(list.removeFirst());
        System.out.println(list.removeFirst());


    }
}

class Node<T> {
    private T elem;
    private Node<T> next;

    public Node( T e, Node<T> n ) {
        this.elem = e;
        this.next = n;
    }

    public T getElement() { return elem; }
    public Node<T> getNext() { return next; }
    public void setNext(Node<T> n) { next = n; }
}