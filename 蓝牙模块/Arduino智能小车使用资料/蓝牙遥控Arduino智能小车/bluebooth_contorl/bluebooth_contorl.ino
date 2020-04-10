//============================亚博========================================
//  智能小车蓝牙遥控实验
//=============================================================================

#define run_car  0x01//按键前
#define back_car  0x05//按键后
#define left_car  0x02//按键左
#define right_car  0x04//按键右
#define stop_car  0x03//按键停
#define left_turn  0x06//按键左旋转
#define right_turn  0x07//按键右旋转
//==============================
//
//车速控制量 control
#define level1  0x08//速度控制标志位1
#define level2  0x09//速度控制标志位2
#define level3  0x0A//速度控制标志位3
#define level4  0x0B//速度控制标志位4
#define level5  0x0C//速度控制标志位5
#define level6  0x0D//速度控制标志位6
#define level7  0x0E//速度控制标志位7
#define level8  0x0F//速度控制标志位8
//==============================
//==============================
int Left_motor_back=9;     //左电机后退(IN1)
int Left_motor_go=5;     //左电机前进(IN2)
int Right_motor_go=6;    // 右电机前进(IN3)
int Right_motor_back=10;    // 右电机后退(IN4)

int control;//PWM控制量

void setup()
{
  //初始化电机驱动IO为输出方式
  pinMode(Left_motor_go, OUTPUT); // PIN 5 (PWM)
  pinMode(Left_motor_back, OUTPUT); // PIN 9 (PWM)
  pinMode(Right_motor_go, OUTPUT); // PIN 6 (PWM)
  pinMode(Right_motor_back, OUTPUT); // PIN 10 (PWM)
  Serial.begin(9600);	//波特率9600 （蓝牙通讯设定波特率）
}
void run()     // 前进
{
  digitalWrite(Right_motor_go,HIGH);  // 右电机前进
  digitalWrite(Right_motor_back,LOW);     
  analogWrite(Right_motor_go,control);//PWM比例0~255调速，左右轮差异略增减
  //analogWrite(Right_motor_back,0);
  digitalWrite(Left_motor_go,HIGH);  // 左电机前进
  digitalWrite(Left_motor_back,LOW);
  analogWrite(Left_motor_go,control-15);//PWM比例0~255调速，左右轮差异略增减
  //delay(time * 100);   //执行时间，可以调整
}

void brake()         //刹车，停车
{
  digitalWrite(Left_motor_back, LOW);
  digitalWrite(Left_motor_go, LOW);
  digitalWrite(Right_motor_go, LOW);
  digitalWrite(Right_motor_back, LOW);
  //delay(time * 100);//执行时间，可以调整
}

void left()         //左转(左轮不动，右轮前进)
{
  digitalWrite(Right_motor_go, HIGH);	// 右电机前进
  digitalWrite(Right_motor_back, LOW);
  analogWrite(Right_motor_go,control);
  //analogWrite(Right_motor_back,0);//PWM比例0~255调速
  digitalWrite(Left_motor_go, LOW);  //左轮不动
  digitalWrite(Left_motor_back, LOW);
  //analogWrite(Left_motor_go,0);
  //analogWrite(Left_motor_back,0);//PWM比例0~255调速
  //delay(time * 100);	//执行时间，可以调整
}

void spin_left()         //左转(左轮后退，右轮前进)
{
  digitalWrite(Right_motor_go, HIGH);	// 右电机前进
  digitalWrite(Right_motor_back, LOW);
  analogWrite(Right_motor_go,control);
  //analogWrite(Right_motor_back,0);//PWM比例0~255调速
  digitalWrite(Left_motor_go, LOW);  //左轮后退
  digitalWrite(Left_motor_back, HIGH);
  //analogWrite(Left_motor_go,0);
  analogWrite(Left_motor_back,control);//PWM比例0~255调速
  //delay(time * 100);	//执行时间，可以调整
}

void right()        //右转(右轮不动，左轮前进)
{
  digitalWrite(Right_motor_go, LOW);  //右电机不动
  digitalWrite(Right_motor_back, LOW);
  //analogWrite(Right_motor_go,0);
  //analogWrite(Right_motor_back,0);//PWM比例0~255调速
  digitalWrite(Left_motor_go, HIGH); //左电机前进
  digitalWrite(Left_motor_back, LOW);
  analogWrite(Left_motor_go,control);
  //analogWrite(Left_motor_back,0);//PWM比例0~255调速
  //delay(time * 100);	//执行时间，可以调整
}

void spin_right()        //右转(右轮后退，左轮前进)
{
  digitalWrite(Right_motor_go, LOW);  //右电机后退
  digitalWrite(Right_motor_back, HIGH);
  //analogWrite(Right_motor_go,0);
  analogWrite(Right_motor_back,control);//PWM比例0~255调速
  digitalWrite(Left_motor_go, HIGH); //左电机前进
  digitalWrite(Left_motor_back, LOW);
  //analogWrite(Left_motor_go,200);
  analogWrite(Left_motor_go,control);//PWM比例0~255调速
  //delay(time * 100);	//执行时间，可以调整
}

void back()          //后退
{
  digitalWrite(Right_motor_go, LOW); //右轮后退
  digitalWrite(Right_motor_back, HIGH);
  //analogWrite(Right_motor_go,0);
  analogWrite(Right_motor_back,control);//PWM比例0~255调速
  digitalWrite(Left_motor_go, LOW); //左轮后退
  digitalWrite(Left_motor_back, HIGH);
  //analogWrite(Left_motor_go,0);
  analogWrite(Left_motor_back,control-15);//PWM比例0~255调速
  //delay(time * 100);     //执行时间，可以调整
}

void loop()
{
   while(Serial.available())
   
  switch (Serial.read())
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
      
    case level1:control=50;break;
    case level2:control=100;break;
    case level3:control=140;break;
    case level4:control=160;break;
    case level5:control=180;break;
    case level6:control=200;break;
    case level7:control=220;break;
    case level8:control=255;break;
      
     default:
       brake();
      break;
  }
}


