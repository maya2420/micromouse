
class motors{
  public:
    motors();
    ~motors();
    //void checkX(); //know where on x
    //void checkY();  // know where on y
    //void count();
    int returnPos_x();
    int returnPos_y();
    int returnEncoder();
    bool goal();

  private:
    int orientation;
    int pos_x;
    int pos_y;
    bool dir_R; //use bool to know direction or could change to int and use num code
    bool dir_L;
    int encoder_count;
};

motors::motors(){
}

motors::~motors(){
}

bool motors::goal(){
  if( (pos_x == 8 || pos_x == 9) && (pos_y == 9 || pos_y == 8) ){
    return true;
  }
  return false;
}

int motors::returnPos_x(){
  return pos_x;
}

int motors::returnPos_y(){
  return pos_y;
}

int motors::returnEncoder(){
  return encoder_count;
}
