
#include <MsTimer2.h>//引入定时器中断头文件
#define run_car  0x02//按键前
#define back_car  0x08//按键后
#define left_car  0x04//按键左
#define right_car  0x06//按键右
#define stop_car  0x05//按键停
#define left_turn  0x07//按键左旋转
#define right_turn  0x09//按键右旋转
#define on_ultrasonic 0x01//On the automatic ultrasonic --Auto Drive Mode
#define off_ultrasonic 0x03// Manual Mode
#define RUN 0
#define LEFT 1
#define RIGHT 2
#define TURNBACK 3
#define AUTOMODE 1
#define MANUALMODE -1

static char * On_ultra="ok"; //蓝牙自动模式指令
static char * Off_ultra="no";//蓝牙手动模式指令
bool isStart=false;//启动
bool UltrasonicOpen=false; // not use
int Left_motor_back = 5;   //左电机后退(IN1) redline
int Left_motor_go = 6;   //左电机前进(IN2)  orangeline

int Right_motor_go = 9;  // 右电机前进(IN3)  yellowline
int Right_motor_back = 10;  // 右电机后退(IN4)greenline

int inPin = 8; // key Detected
int outPin = 11; // ledControl
int val;// Read Key Status

int myangle;//定义角度变量  0-180
int pulsewidth;//定义脉宽变量 0.5-2.5ms
int servopin = 3;

int echoPin = A3; // Echo(P2.0) purpleline
int trigPin = A4; //  Trig 触发脚(P2.1) yellowline
float Distance = 0;  //cm
float left_Distance; //left
float right_Distance;//right

float DetectPrecison=30.0; //识别距离
int Action; //自动模式下的动作
int Mode;//运行模式
void setup() {
  // put your setup code here, to run once:
  //初始化电机驱动IO为输出方式
  pinMode(Left_motor_go, OUTPUT); // PIN 6 (PWM)
  pinMode(Left_motor_back, OUTPUT); // PIN 5 (PWM)
  pinMode(Right_motor_go, OUTPUT); // PIN 9 (PWM)
  pinMode(Right_motor_back, OUTPUT); // PIN 10 (PWM)
  pinMode(outPin, OUTPUT); //PIN 11
  pinMode(inPin, INPUT); //PIN 8
  pinMode(servopin, OUTPUT); //PIN 3
  pinMode(echoPin, INPUT);    // 定义超声波输入脚
  pinMode(trigPin, OUTPUT);   // 定义超声波输出脚
  Serial.begin(9600);//设置波特率为9600
  Serial.println("servo=o_seral_simple ready" );
}
void servopulse(int servopin, int myangle) /*定义一个脉冲函数，用来模拟方式产生PWM值*/
{
  pulsewidth = (myangle * 11) + 500; //将角度转化为500-2480 的脉宽值
  digitalWrite(servopin, HIGH); //将舵机接口电平置高
  delayMicroseconds(pulsewidth);//延时脉宽值的微秒数
  digitalWrite(servopin, LOW); //将舵机接口电平置低
  delay(20 - pulsewidth / 1000); //延时周期内剩余时间
}
float Distance_test()   // 量出前方距离
{
  digitalWrite(trigPin, LOW);   // 给触发脚低电平2μs
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);  // 给触发脚高电平10μs，这里至少是10μs
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);    // 持续给触发脚低电
  float Fdistance = pulseIn(echoPin, HIGH);  // 读取高电平时间(单位：微秒)
  Fdistance = Fdistance / 58;    //为什么除以58等于厘米，  Y米=（X秒*344）/2
  // X秒=（ 2*Y米）/344 ==》X秒=0.0058*Y米 ==》厘米=微秒/58
  Serial.print("Distance:");      //输出距离（单位：厘米）
  Serial.println(Fdistance);         //显示距离
  return Fdistance;
}
void run()
{
  digitalWrite(Right_motor_go, HIGH); // 右电机前进
  digitalWrite(Right_motor_back, LOW);
  analogWrite(Right_motor_go, 150); //PWM比例0~255调速，左右轮差异略增减
  analogWrite(Right_motor_back, 0);
  digitalWrite(Left_motor_go, HIGH); // 左电机前进
  digitalWrite(Left_motor_back, LOW);
  analogWrite(Left_motor_go, 150); //PWM比例0~255调速，左右轮差异略增减
  analogWrite(Left_motor_back, 0);
  //delay(time * 100);   //执行时间，可以调整
}
void brake()         //刹车，停车
{
  digitalWrite(Right_motor_go, LOW);
  digitalWrite(Right_motor_back, LOW);
  digitalWrite(Left_motor_go, LOW);
  digitalWrite(Left_motor_back, LOW);
  //delay(time * 100);//执行时间，可以调整
}

void left()         //左转(左轮不动，右轮前进)
{
  digitalWrite(Right_motor_go, HIGH);	// 右电机前进
  digitalWrite(Right_motor_back, LOW);
  analogWrite(Right_motor_go, 200);
  analogWrite(Right_motor_back, 0); //PWM比例0~255调速
  digitalWrite(Left_motor_go, LOW);  //左轮不动
  digitalWrite(Left_motor_back, LOW);
  analogWrite(Left_motor_go, 0);
  analogWrite(Left_motor_back, 0); //PWM比例0~255调速
 // delay(time * 100);	//执行时间，可以调整
}

void spin_left()         //左转(左轮后退，右轮前进)
{
  digitalWrite(Right_motor_go, HIGH);	// 右电机前进
  digitalWrite(Right_motor_back, LOW);
  analogWrite(Right_motor_go, 200);
  analogWrite(Right_motor_back, 0); //PWM比例0~255调速
  digitalWrite(Left_motor_go, LOW);  //左轮后退
  digitalWrite(Left_motor_back, HIGH);
  analogWrite(Left_motor_go, 0);
  analogWrite(Left_motor_back, 200); //PWM比例0~255调速
  //delay(time * 100);	//执行时间，可以调整
}

void right()        //右转(右轮不动，左轮前进)
{
  digitalWrite(Right_motor_go, LOW);  //右电机不动
  digitalWrite(Right_motor_back, LOW);
  analogWrite(Right_motor_go, 0);
  analogWrite(Right_motor_back, 0); //PWM比例0~255调速
  digitalWrite(Left_motor_go, HIGH); //左电机前进
  digitalWrite(Left_motor_back, LOW);
  analogWrite(Left_motor_go, 200);
  analogWrite(Left_motor_back, 0); //PWM比例0~255调速
  //delay(time * 100);	//执行时间，可以调整
}

void spin_right()        //右转(右轮后退，左轮前进)
{
  digitalWrite(Right_motor_go, LOW);  //右电机后退
  digitalWrite(Right_motor_back, HIGH);
  analogWrite(Right_motor_go, 0);
  analogWrite(Right_motor_back, 200); //PWM比例0~255调速
  digitalWrite(Left_motor_go, HIGH); //左电机前进
  digitalWrite(Left_motor_back, LOW);
  analogWrite(Left_motor_go, 200);
  analogWrite(Left_motor_back, 0); //PWM比例0~255调速
  //delay(time * 100);	//执行时间，可以调整
}

void back()          //后退
{
  digitalWrite(Right_motor_go, LOW); //右轮后退
  digitalWrite(Right_motor_back, HIGH);
  analogWrite(Right_motor_go, 0);
  analogWrite(Right_motor_back, 150); //PWM比例0~255调速
  digitalWrite(Left_motor_go, LOW); //左轮后退
  digitalWrite(Left_motor_back, HIGH);
  analogWrite(Left_motor_go, 0);
  analogWrite(Left_motor_back, 150); //PWM比例0~255调速
  //delay(time * 100);     //执行时间，可以调整
}
//下面是对动作函数的重载
void back(int time)
{
  back();
  delay(time*100);
}
void left(int time)
{
  left();
  delay(time*100);
}
void right(int time)
{
  right();
  delay(time*100);
}
void spin_left(int time)
{
  spin_left();
  delay(time*100);
}
void spin_right(int time)
{
  spin_right();
  delay(time*100);
}
void brake(int time)
{
  brake();
  delay(time*100);
}
//转向探测
void turn_right()
{
  val = 0 ; //There is a deviation so i add 20 degree on it ;
  Serial.print(val - 20, DEC);
  for (int i = 0; i <= 50; i++) //产生PWM个数，等效延时以保证能转到响应角度
  {
    servopulse(servopin, val); //模拟产生PWM
  }
  right_Distance=Distance_test();
}
void turn_left()
{
  val = 180; //There is a deviation so i add 10 degree on it ;
  Serial.print(val - 10, DEC);
  for (int i = 0; i <= 50; i++) //产生PWM个数，等效延时以保证能转到响应角度
  {
    servopulse(servopin, val); //模拟产生PWM
  }
  left_Distance=Distance_test();
}
//转向前方同时探测距离
void turn_forward()
{
   val = 90 ; //There is a deviation so i add 10 degree on it ;
  //Serial.print(val - 10, DEC);
  for (int i = 0; i <= 50; i++) //产生PWM个数，等效延时以保证能转到响应角度
  {
    servopulse(servopin, val); //模拟产生PWM
  }
  Distance=Distance_test();
}
//超声波---此函数未使用
void ultrasonic()
{
  turn_forward();//face the front
    if (Distance<DetectPrecison)
     {
       back(2);//后退减速0.2s
       brake(2);//停下来做测距0.2s
       turn_left();
       turn_right();
      if((left_Distance < DetectPrecison ) &&( right_Distance < DetectPrecison ))//当左右两侧均有障碍物靠得比较近
        spin_left(0.7);//旋转掉头
      else if(left_Distance>right_Distance)//左边空旷
      {
        left(3);
        brake(1);
      }
      else // 右边空旷
      {
        right(3);
        brake(1);
      }
     }
     else  //直行
       {
         run();
       }
}
//中断服务程序
void InterServe()
{
  //step1:
    turn_forward();
    if (Distance<DetectPrecison)
     {
       back(2);//后退减速0.2s
       brake(2);//停下来0.2s
       turn_left();//左转探测
       turn_right();//右转
        if((left_Distance < DetectPrecison ) &&( right_Distance < DetectPrecison ))//当左右两侧均有障碍物靠得比较近
        //spin_left(0.7);//旋转掉头
          Action=TURNBACK;
        else if(left_Distance>right_Distance)//左边空旷
        {
        //left(3);
        //brake(1);
          Action=LEFT;
         }
        else // 右边空旷
        {
        //right(3);
        //brake(1);
        Action=RIGHT;
        }
     }
   else  //直行
       {
         Action=RUN;
       }
  //step2:串口控制模式改变响应
       while (Serial.available())//检查蓝牙串口有无数据
       //switch (Serial.read())//读取数据
       {
         if(Serial.find(On_ultra));  //预读取"on"
         {
             Mode=AUTOMODE;
             Serial.flush();
             break;
         }
          if(Serial.find(Off_ultra)); //预读取"off"
         {
             Mode=MANUALMODE;
             Serial.flush() ;
             break;
         }
       }
}
void loop() {
  
  if(digitalRead(inPin)==HIGH)//等待开关按下启动
  {
    isStart=true;
    digitalWrite(outPin,HIGH);
    turn_forward();//face the front 
    MsTimer2::set(1000, InterServe); // 1000ms period
    MsTimer2::start();//启动定时器中断
    Mode=AUTOMODE; //初始为自动模式
    Action=RUN;//前进
  }
  while(isStart)
  {
    if(Mode==AUTOMODE)
    {
      switch(Action)//根据中断检测后的指示执行相应操作
      {
        case RUN:
          run();
          break;
        case TURNBACK:
          spin_left(7);//旋转掉头0.7s
          brake(3);//暂停0.3s
        break;
        case LEFT:
          left();
          break;
        case RIGHT:
          right();
          break;
        default:
          run();
      }
    }
    else  //ManualMode
    {
      while (Serial.available())//检查蓝牙串口有无数据
      switch (Serial.read())//读取数据
      {
      case run_car:
        run();
        break;
      case back_car:
        back();
        break;
      case left_car:
        left();
        break;
      case right_car:
        right();
        break;
      case   stop_car:
        brake();
        break;
      case left_turn:
        spin_left();
        break;
      case right_turn:
        spin_right();
        break;
      default:
        brake();
        Serial.print("Stop");
        break;
       }
    }
  }

}
