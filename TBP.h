#ifndef TBP_H
#define TBP_H 

class TBP{
   private:
  int state;
  public:
  TBP(){
    this->state = 2;
  }
  void update(bool result){
        if(result ==1 && state == 0){
            state =0;
        }
        else if(result == 0 && state ==0){
            state =1;
        }
        else if(result == 0 && state ==1){
            state = 2;
        }
        else if(result == 1 && state ==1){
            state =0;
        }
        else if(result == 0 && state ==2){
            state =3;
        }
        else if(result == 1 && state ==2){
            state =1;
        }
        else if(result == 0 && state ==3){
            state =3;
        }
        else if(result == 1 && state ==3){
            state =2;
        }
  }
 bool prediction(){
    if(state == 0 ||state ==1){
        return true;
    }
    else if (state == 2|| state ==3){
        return false;
    }
    return false;
 }
  
};
#endif