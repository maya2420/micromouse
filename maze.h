
struct node {
  node* next = nullptr;
  node* prev = nullptr;
  int value;

  node(int val){ //node
    next = nullptr;
    prev = nullptr;
    value = val;
  }
};

struct path { //connection between nodes
  int end;
  int start;
  int distance;
};

class maze {
public:
  maze(path p[], int num);

private:
  node** head;
  const int n = 256;
};

maze::maze(path p[], int num) {
  head = nullptr;
  node* point[n];

  maze* m = (maze*) malloc (sizeof (maze));
}


