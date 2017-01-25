#include <Arduino.h>
#include <Sparkfun_Tlc5940.h>


//pdm matrix
const int MATRIX_STATES_NUM = 6; //thre are 6 matrix status
const int MATRIX_EVAL_NUM = 4;
const int MATRIX_SLOT_NUM = 3;// there are 3 slots avaliable
float pdm_state_eval_matrix[MATRIX_STATES_NUM][4][3] = {
  //S1 : e0, e1, e2, e3
  {
    {1, 1, 1}, {1.5, 0.75, 0.75},{0.75, 1.5, 0.75}, {0.75, 0.75, 1}
  },

  //S2
  {
    {0.75, 0.75, 1.5}, {/*>>*/1, 0.5, 1.5}, {0.5, /*>>*/1, 1.5}, {0.5, 0.5, /*>>*/2}
  },

  //S3
  {
    {0.5, 0.5, 2}, {/*>>*/1, 0, 2}, {0, /*>>*/1 , 2}, {1, 1, /*>>*/1}
  },

  //S4
  {
    {0.5, 1, 1.5}, {/*>>*/1, 1, 1}, { 0, /*>>*/1.5, 1.5}, {0, 1, /*>>*/2}
  },

  //S5
  {
    /*confusion with 0 transform, need for deeper experiment*/
    {0, 1, 2}, {/*>>*/0.5, 1, 2}, { 0, /*>>*/1.5, 1.5}, { 1, 1, /*>>*/1}
  },

  //S6
  {
    {0, 1.5, 1.5}, {/*>>*/1, 1, 1}, {0, /*>>*/2, 1},{0, 1, /*>>*/2}
  }
};


//struct

struct k_info {
  float k_arr_current[3]; //store the current k array data, will update intime
  float k_arr_sorted[3]; //store the sorted k array, always with update of current
  int k_arr_sorted_pos[3]; //store the sorted position of k array
};
struct k_info k_info_ins;
float *k_arr_current_ptr = k_info_ins.k_arr_current;
float *k_arr_sorted_ptr = k_info_ins.k_arr_sorted;
int *k_arr_sorted_pos_ptr = k_info_ins.k_arr_sorted_pos;
const int K_NUM = 3;

//print entire float array for debug purpose
void debug_print_arr(float* array, int size){
  for (int i=0; i<size; i++){
    Serial.print(*(array+i));
    Serial.print(',');
  }
  Serial.println();
}
void debug_print_arr_int(int* array, int size){
  for (int i=0; i<size; i++){
    Serial.print(*(array+i));
    Serial.print(',');
  }
  Serial.println();
}




/*Sort k incremently and return the position value after buble sorted.
  k_info is a struct contain k value it's corresponding position values
 */
void k_struct_init(){
  Serial.println("Start init K array");
  /*get current_k_value*/
  *k_arr_current_ptr = 1.0;
  *(k_arr_current_ptr+1) = 1.5;
  *(k_arr_current_ptr+2) = 0.5;
  /*initialise the position array & sorted array*/
  for (int i=0; i<K_NUM;i++){
     *(k_arr_sorted_pos_ptr+i) = i+1;
     *(k_arr_sorted_ptr+i) = *(k_arr_current_ptr+i);
  }

  /*sort array incremently with buble sort*/
  Serial.println("Before sort");
  debug_print_arr(k_arr_current_ptr,K_NUM);
  debug_print_arr_int(k_arr_sorted_pos_ptr,K_NUM);
  for (int i=0; i<(K_NUM-1); i++){
    for (int j=0; j<(K_NUM-(i+1)); j++){
     if (*(k_arr_sorted_ptr+j) > *(k_arr_sorted_ptr+j+1)) {
       //swap
       float temp = *(k_arr_sorted_ptr+j);
       *(k_arr_sorted_ptr+j) = *(k_arr_sorted_ptr+j+1);
       *(k_arr_sorted_ptr+j+1) = temp;
       //the position also swap according to the array
       int t = *(k_arr_sorted_pos_ptr+j);
       *(k_arr_sorted_pos_ptr+j) = *(k_arr_sorted_pos_ptr+j+1);
       *(k_arr_sorted_pos_ptr+j+1) = t;
     } else {
       //do nothing
     }
   }
  }
  Serial.println("after sort");
  debug_print_arr(k_arr_sorted_ptr, K_NUM);
  debug_print_arr_int(k_arr_sorted_pos_ptr,K_NUM);
  Serial.println("===");

}






/*MAIN*/

void setup() {
  Serial.begin(9600);
}


void loop(){
  k_struct_init();
}
