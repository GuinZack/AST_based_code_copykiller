// Definition of queue node
typedef struct t_queue_node {
    int vertex;
    string data;
    struct t_queue_node *link;   
} queue_node;

// Definition of Queue class
class Queue {
  private:
    queue_node *front;
    queue_node *rear;

  public:
    Queue();            
    ~Queue();           
    void delete_stack(queue_node *node_ptr); 
    void AddQ(string data);
    string  DeleteQ();
    void Print();
    bool IsEmpty();
};

Queue::Queue()
{
    front = NULL;
    rear = NULL;
}

Queue::~Queue()    // recursive approach
{	
    delete_stack(front);
}

void Queue::delete_stack(queue_node* node_ptr) 
{
    if (node_ptr != NULL) 
    {
        delete_stack(node_ptr->link);
        delete node_ptr; 
    }
}

void Queue::AddQ(string data)
{
    queue_node *new_node = new queue_node;

    if (new_node == NULL)
        return;

    new_node->data = data;
    new_node->link = NULL;
    if (front) 
        rear->link = new_node;       
    else 
        front = new_node; // queue is empty
    
    rear = new_node; 
}


string Queue::DeleteQ()
{
    string data;
    queue_node *del = NULL;
    if (front == NULL)
        return NULL;

    data = front->data;
    del = front;
    front = front->link;
    delete del;
    
    return data;
}


void Queue::Print()
{
    queue_node *cur;
    cout << "Queue: ";
    for (cur = front; cur != NULL; cur = cur->link)
    {
        cout << cur->data << "  ";
    }
    cout << endl;
}


bool Queue::IsEmpty() 
{
    return (front == NULL);
}