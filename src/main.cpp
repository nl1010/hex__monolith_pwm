#include <Arduino.h>
#include <Sparkfun_Tlc5940.h>

/*Binary information of k after sort k
e.g.  1,1.5,0.5---inc_sort---> {{0.5,1,1.5},{3,1,2}}
*/
float k_bi_info[2][3];

struct k_info {
  float *k_sorted;
  int *k_pos;
};



float pdm_state_eval_matrix[6][4][3] = {
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



/*return current power diversion management status from each port
  data structure: {I, II, III}
*/
float *get_current_pdm_status(){
  // demo
  // return current powerstatus
  /*fix: about returning an array*/
  static float array[3] = {1.5,0,1.5};
  return array;
}


void pdm_mapping(){
  //  float *k_img = get_current_pdm_status();
}


//print entire float array for debug purpose
void debug_print_arr(float array[], int size){
  for (int i=0; i<size; i++){
    Serial.print(array[i]);
    Serial.print(',');
  }
  Serial.println();
}

void debug_print_arr_int(int array[], int size){
  for (int i=0; i<size; i++){
    Serial.print(array[i]);
    Serial.print(',');
  }
  Serial.println();
}


/*Sort k incremently and return the position value after buble sorted.
  k_info is a struct contain k value it's corresponding position values
 */
struct k_info sort_k_inc(float array[], int arr_size){
  struct k_info k_info_ins; //struct instance init
  //sort array incremently with buble sort
  // Serial.println("before sort");
  // debug_print_arr(array,arr_size);
  int position_arr[arr_size];
  for (int i=0; i<(arr_size);i++){
     position_arr[i] = i+1;
  }
  // float new_array[arr_size];
  for (int i=0; i<(arr_size-1); i++){
    for (int j=0; j<(arr_size-(i+1)); j++){
     if (array[j] > array[j+1]) {
       //swap
       float temp = array[j];
       array[j] = array[j+1];
       array[j+1] = temp;
       //the position also swap according to the array
       int t = position_arr[j];
       position_arr[j] = position_arr[j+1];
       position_arr[j+1] = t;
     } else {
       //do nothing
     }
   }
  }
  // Serial.println("after sort");
  // debug_print_arr(array,arr_size);
  // debug_print_arr_int(position_arr,arr_size);
  // Serial.println();

  k_info_ins.k_sorted = array;
  k_info_ins.k_pos = position_arr;

  return k_info_ins;
}




/// main


void setup() {
  Serial.begin(9600);
}





void loop(){
  // Serial.println(pdm_state_eval_matrix[0][0][0]);
  // Serial.println(pdm_state_eval_matrix[2][2][2]);
  float array[] = {2, 1, 0,5,6,1.7};
  int size = sizeof(array)/sizeof(float);
  debug_print_arr(array,size);
  struct k_info arr = sort_k_inc(array,size);
  Serial.print("--");
  debug_print_arr_int(arr.k_pos,size);
  debug_print_arr(arr.k_sorted,size);
}
