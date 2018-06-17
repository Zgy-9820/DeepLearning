/*******************************************************************************
* 描    述 :参数声明
* 隶属模块 : 
* 输入参数 : 
* 返回参数 : 
*******************************************************************************/
boolean isAvailable = false; //接收数据的标志
char serialRead;
int led = 13;
int i=1;
int value = 0;
int left_value = 156;
int right_value = 156;
int start = false;   //停止
int num=0;
//PWM
int pin_left = 12;
int pin_right = 11;
/*******************************************************************************
* 描    述 :led闪烁
* 作    用 : 调试作用
* 输入参数 :
* 返回参数 : 无
*******************************************************************************/
void led_flow(void){
  //设置数字口13为高电平，此时LED常亮
  digitalWrite(led,HIGH);
  //延时0.5s
  delay(500);
  digitalWrite(led,LOW);
  //延时0.5s
  delay(500);  
}
/*******************************************************************************
* 描    述 :延时函数 100um
* 隶属模块 : 
* 输入参数 : 0.5ms = 5,1ms = 10,1.5ms = 15,2ms = 20,2.5ms = 25,10ms = 100,20ms = 200
* 返回参数 : 无
*******************************************************************************/
void millisecond_100(int N_time){  
   for(int i = 0;i<N_time;i++){
        delayMicroseconds(100); 
    }  
}

/*******************************************************************************
* 描    述 :延时函数 10um
* 隶属模块 : 
* 输入参数 : 0.5ms = 50,1ms = 100,1.5ms = 150,2ms = 200,2.5ms = 250,10ms = 1000,20ms = 2000
* 返回参数 : 无
*******************************************************************************/
void millisecond_10(long N_time){  
   for(long i = 0;i<N_time;i++){
        delayMicroseconds(10); 
    }  
}

/*******************************************************************************
* 函数作用 : pwm初始化 ,可以用在停车函数
* 函数描述 : 发送一个1.5ms的校准信号
* 输入参数 : 
* 返回参数 : 
*******************************************************************************/
void pwmInit()
{
    pwm_right(156);
    pwm_left(156);  
    left_value = 156;
    right_value = 156;
}   

/*******************************************************************************
* 函数描述 :  全部初始化
* 隶属模块 : 
* 输入参数 : 
* 返回参数 : 
*******************************************************************************/
void setup() 
{
    pinMode(pin_left, OUTPUT);
    pinMode(pin_right, OUTPUT);
    pinMode(led, OUTPUT);
    Serial.begin(9600);  //设置波特率 
    pinMode(led,OUTPUT);
    pwmInit();
    led_flow();
}

/*******************************************************************************
* 函数描述 : 小车控制部分
* 隶属模块 : 
* 输入参数 : 
* 返回参数 : 
*******************************************************************************/
void carControl(int num){
  if(num == 5){
    start = true;
    pwmInit();
    led_flow();  // 闪烁指示初始化完毕
    }else if(num == 6){
      start = false;
      pwmInit();
     }
   if(start)  //开始执行
   {
      if(num>=49&&num<=251)  // 左边
      {
         left_value = num;
      }else if(num>=259&&num<=461)  // 右边
      {
          right_value = num - 210;  //发送从 260-460，最后减去210从而得到
      }else if(num == 12)  //前进
      {
          car_forward();
          pwmInit();    
       }
       else if(num == 15) //后退
      {
          car_back();
          pwmInit();
        
       }else if(num == 13)  //左转
      {
          car_left();
          pwmInit();
          
       }else if(num == 14)  //右转
      {
          car_right();
          pwmInit();
      }

   } 
}
/*******************************************************************************
* 函数描述 : 主程序
* 隶属模块 : 
* 输入参数 : 
* 返回参数 : 
*******************************************************************************/
void loop() 
{
    readSerial();//检查串口是否有数据过来，有就保存在serialRead中
    Isdigit();
}

/*******************************************************************************
* 函数作用 : 调节pwm输出
* 函数描述 : high_time= 50(反转最快)，100（反转中等速度），150(停止)，200(正传中等速度)，250(正转最快速度)
* 输入参数 : high_time = ( 0.5ms = 50,1ms = 100,1.5ms = 150,2ms = 200,2.5ms = 250,10ms = 1000,20ms = 2000 )
* 返回参数 : 
*******************************************************************************/
void pwm_left(int high_time){  //左轮
  digitalWrite(pin_left, HIGH);
  millisecond_10(high_time);
  digitalWrite(pin_left, LOW);  
  millisecond_10(2080-high_time);
}

void pwm_right(int high_time){   
  digitalWrite(pin_right, HIGH);
  millisecond_10(high_time);
  digitalWrite(pin_right, LOW);  
  millisecond_10(2080-high_time);
}

/*******************************************************************************
* 函数作用 : 小车控制
* 函数描述 : 总的小车控制函数（前进，后退，左转，右转 ），每次点击左转或者右转都会转一定的角度然后停下来
* 输入参数 : left(左轮速度,140最慢向前，160最慢向后)，right(右轮速度，140向前最慢，160向后最慢) 
* 返回参数 : 
*******************************************************************************/
void car(int left,int right){
    pwm_left(left);
    pwm_right(right);
}
// 向左（左轮向后，右轮向前）
void car_left(){
  car(160,140);
  delay(50);
}

// 向右（左轮向前，右轮向后）
void car_right(){
  car(140,160);
  delay(50);
}

// 前进（两个轮子都前进）
void car_forward(){
  car(140,140);
  delay(50);
}
// 后退
void car_back(){
  car(160,160);
  delay(50);
}

/*******************************************************************************
* 函数描述 : 把PC端发送的三位字符转换成数字，把要执行的函数放到这个函数
* 隶属模块 : 
* 输入参数 : 
* 返回参数 : 
*******************************************************************************/
void Isdigit()
{
  if(isAvailable)
  {
      unsigned char c = serialRead;
      if(i==1)
      {
            num = num + (c - '0')*100;
            i++;
      }
      else if(i==2)
      {
          num = num + (c - '0')*10;
          i++;
      }
      else if(i==3)
      {
         num = num + (c - '0');
         i=1;
         /*******************************
          * 控制执行函数
          *******************************/

          carControl(num);  
            
        /*******************************/
        num = 0;
      }
  } 
  else
  {
    car(left_value,right_value);  // 如果没有数据发送过来，则执行上一次保留的速度值
  }
}


/*******************************************************************************
* 函数描述 : 串口获取数据函数。
* 隶属模块 : 
* 输入参数 : 
* 返回参数 : 
*******************************************************************************/
void readSerial()  //读串口缓冲区函数
{
 isAvailable = false;/*在读取串口时就把状态机置为false */ 
 if(Serial.available()>0)
 {
   isAvailable = true;
   serialRead = Serial.read(); /*读取得数据存入serialRead中*/  
 } 
}
