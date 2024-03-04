class IR_sensors{

  public:
    IR_sensors(); 
    ~IR_sensors();
    void check_fR(int);
    void check_fL(int);
    void check_sR(int);
    void check_sL(int);
    bool returnFR();
    bool returnFL();
    bool returnSR();
    bool returnSL();

  private:
    bool stop_fR;
    bool stop_fL;
    bool stop_sR;
    bool stop_sL;
};

IR_sensors::IR_sensors(){
}

IR_sensors::~IR_sensors(){
}

void IR_sensors::check_fR(int front_R){
  if(front_R <= 900 ){
    stop_fR = false;
  }
    else{
      stop_fR = true;
    }
}

void IR_sensors::check_fL(int front_L){
  if(front_L <= 900 ){
    stop_fL = false;
    //Serial.println(" fL: false");
  }
    else{
      stop_fL = true;
      //Serial.println("fL: true");
    }
}

void IR_sensors::check_sR(int side_R){
  if(side_R <= 950 ){
    stop_sR = false;
  }
    else{
      stop_sR = true;
    }
}

void IR_sensors::check_sL(int side_L){
  if(side_L <= 950 ){
    stop_sL = false;
  }
  else{
    stop_sL = true;
  }
}

bool IR_sensors::returnFR(){
  return stop_fR;
}

bool IR_sensors::returnFL(){
  return stop_fL;
}

bool IR_sensors::returnSL(){
  return stop_sL;
}

bool IR_sensors::returnSR(){
  return stop_sR;
}
