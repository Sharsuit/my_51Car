# 基于51单片机实现的寻迹小车

## 一、硬件连接

### 1.所需模块

1. L298N 电机驱动模块

2. SPP蓝牙透传模块

3. 51单片机最小系统板

### 2. 引脚定义

#### 公共引脚

- `VCC` 单片机5V供电模块

- `GND` 共用地线接口

- `+12` 锂电池供电接口

- `+5V` L298N 对单片机的供电接口

#### 单片机引脚

型号STC89C52RC  晶振11.0592Mhz

- `P1^0` PWM调频左

- `P1^1` PWM调频左

- `P1^2 ~ P1^6` 从左到右的寻迹IO口

- `P3^0 P3^1` 串口通信的RXD TXD（注意连接方式是交叉连接）

- `P3^2 ~ P3^5` 调速独立按键 接地触发

- `P0^0 P0^1` 控制右方的两个轮子

- `P0^2 P0^3` 控制左方的两个轮子
  
  ```c
  #include <REGX52.H>
  #include "Delay.h"
  //#include "LCD1602.h"
  #include "Timer0.h"
  #include "UART.h"
  //sbit Motor=P1^0;
  sbit Motor1=P1^0;
  sbit Motor2=P1^1;
  sbit MR=P0^0;
  sbit NR=P0^1;
  sbit ML=P0^2;
  sbit NL=P0^3;
  //循迹
  int dect=0;
  sbit L2=P1^2; 
  sbit L1=P1^3; 
  sbit C0=P1^4; 
  sbit R1=P1^5; 
  sbit R2=P1^6; 
  
  sbit Led1=P0^4;
  sbit Led2=P2^0;
  sbit Led3=P2^1;
  sbit Led4=P2^2;//状态灯
  
  sbit Swi =P0^5;
  sbit Beez = P0^6;
  unsigned char Sp1=25,Sp2=30    ,Sp3=40,Sp4=45;
  unsigned char ctrl[3];
  //电容赛道,停止线,横线转向引导
  int num=0;
  //unsigned char leader[]={'0','W','W','P'};//第一位备用
  //unsigned char leader[]={'0','W','W','W','W','P'};//第一位备用
  unsigned char leader[]={'0','W','W','W','A','W','P'};
  
  unsigned char Counter,Compare,Compare1,Compare2;    
  unsigned char KeyNum,Speed;
  int previous=4,current=4;
  ```

### 3. 连接示意图











## 二、模块原理

### 略(自己查)

## 三、代码实现

首先设置`ctrl[1]`的值，然后调用`FOMAL_RUN()`，该函数会自动根据数组的值设置引脚控制小车轮子的运行状态。使用WASD控制小车运行状态，方便理解。

```c
void FORMAL_RUN()
{
    if(ctrl[1]=='W')
    {
    ML=0;
    NL=1;
    MR=0;
    NR=1;
    }
    else if(ctrl[1]=='A')
    {
    ML=1;
    NL=0;
    MR=0;
    NR=1;
    }else if(ctrl[1]=='S')
    {
    ML=1;
    NL=0;
    MR=1;
    NR=0;
    }else if(ctrl[1]=='D')
    {
    ML=0;
    NL=1;
    MR=1;
    NR=0;
    }else if(ctrl[1]=='a')
    {
    ML=0;
    NL=0;
    MR=0;
    NR=1;
    }else if(ctrl[1]=='d')
    {
    ML=0;
    NL=1;
    MR=0;
    NR=0;
    }        
    else
    {
    MR=0;
    NR=0;
    ML=0;
    NL=0;    
    }
}
```

AutoRun()专用于寻迹控制小车的运行状态 ，与蓝牙控制类似，但与蓝牙区分开，方便对寻迹的控制。saidao()用于更改赛道

```c
void AutoRun(char txxt)
{

    switch(txxt)
        {
        case'W':ctrl[1]='W';FORMAL_RUN();break;
        case'w':ctrl[1]='W';FORMAL_RUN();break;
        case'A':ctrl[1]='A';FORMAL_RUN();break;
        case'Q':ctrl[1]='W';FORMAL_RUN();break;
        case'S':ctrl[1]='S';FORMAL_RUN();break;
        case'D':ctrl[1]='D';FORMAL_RUN();break;
        case'E':ctrl[1]='W';FORMAL_RUN();break;
        case'a':ctrl[1]='a';FORMAL_RUN();break;
        case'd':ctrl[1]='d';FORMAL_RUN();break;
        case'P':ctrl[1]='P';FORMAL_RUN();break;
        case'1':Compare1=10;Compare2=10;break;
        case'2':Compare1=20;Compare2=20;break;
        case'3':Compare1=30;Compare2=30;break;
        case'4':Compare1=40;Compare2=40;break;
        case'5':Compare1=50;Compare2=50;break;
        case'6':Compare1=60;Compare2=60;break;
        case'7':Compare1=70;Compare2=70;;break;
        case'8':Compare1=80;Compare2=80;break;
        }

}
void saidao(char a)
{
    if(a==1)
    {
        num=0;
        leader[0]='0';
        leader[1]='W';
        leader[2]='W';
        leader[3]='P';
        leader[4]='0';
        leader[5]='0';
        leader[6]='0';
        Led2=1;
        Led3=1;
        Led4=1;
        Led2=0;
        Beez=0;Delay(100);Beez=1;

    }
    else if(a==2)
    {
        num=0;
        leader[0]='0';
        leader[1]='W';
        leader[2]='W';
        leader[3]='W';
        leader[4]='W';
        leader[5]='P';
        leader[6]='0';
        Led2=1;
        Led3=1;
        Led4=1;
        Led2=0;
        Led3=0;
        Beez=0;Delay(100);Beez=1;
    }
    else if(a==3)
    {
        num=0;
        leader[0]='0';
        leader[1]='W';
        leader[2]='W';
        leader[3]='W';
        leader[4]='A';
        leader[5]='W';
        leader[6]='P';
        Led2=1;
        Led3=1;
        Led4=1;
        Led2=0;
        Led3=0;
        Led4=0;
        Beez=0;Delay(100);Beez=1;
    }
}
```

update()用于获取寻迹模块检测到的寻迹数据，合成到一个整数上，这样作可以简化代码，方便调用，后续的while卡循环需要`update()`函数来实时不断判断寻迹模块的状态，简化逻辑。在识别电容赛道时，结合C语言的与运算，发挥及其关键的作用。

changer() 作为以寻迹状态改变为事件的触发函数，可以辅助解决一些问题，但在本程序中用到的不多。

```c
int update()
{
    L2?(dect=dect|0x10):(dect=dect&0x0F);
    L1?(dect=dect|0x08):(dect=dect&0x17);
    C0?(dect=dect|0x04):(dect=dect&0x1B);
    R1?(dect=dect|0x02):(dect=dect&0x1D);
    R2?(dect=dect|0x01):(dect=dect&0x1E);
    0?(dect=dect|0x00):(dect=dect&0xFF);
    return dect;
}
int Changer()
{
    previous=current;
    current=update();
    if(current==previous)
    {
        return 1;
    }
    return 0;
}
```

寻迹函数，需要循环执行。

1. `doLine()`用于检测到横线后执行操作。

2. 使用`ctrl[0]`的值控制寻迹的开关，禁用寻迹后可以自由使用蓝牙进行控制。

3. 寻迹模块分5路检测，检测到白色返回1，检测到黑线返回0，在我们的程序中使用一个`int`类型`dect`和`update()`的返回值作为寻迹数据。

4. 这个返回值可以看做一个二进制数据，忽略高位，只看低5位。

5. 当中路检测到黑线时，返回的数据为 `11011` 对应的十六进制为`0x1B`小车为直行状态。

6. 当左一路检测到黑线时，返回的数据为 `10111` 对应的十六进制为`0x17`应让小车偏右行驶

7. 当右一路检测到黑线时，返回的数据为 `11101` 对应的十六进制为`0x1D`应让小车偏左行驶

8. 当右侧路检测到黑线时，返回的数据为 `11100` 对应的十六进制为`0x1C`应让小车右转弯行驶

9. 当右侧路检测到黑线时，返回的数据为 `11000` 对应的十六进制为`0x18`应让小车右转弯行驶

10. 当左侧路检测到黑线时，返回的数据为 `00111` 对应的十六进制为`0x07`应让小车左转弯行驶

11. 当左侧路检测到黑线时，返回的数据为 `00011` 对应的十六进制为`0x03`应让小车左转弯行驶

```c
void doLine()
{
    if(leader[num]!='P')
    {
    num+=1;
    }
    switch(leader[num])
    {
        case'W':Compare1=Sp1;Compare2=Sp1;AutoRun('W');Beez=0;Led1=0;update();while(!(update()==0x1F||update()==0x1B));break;//---
        case'D':Compare1=Sp1;Compare2=Sp1;AutoRun('D');Beez=0;Led1=0;Delay(1000);while(update()!=0x1B);break;
        case'A':Compare1=Sp1;Compare2=Sp1;AutoRun('A');Beez=0;Led1=0;Delay(1000);while(update()!=0x1B);break;
        case'P':AutoRun('P');Led1=0;Beez=1;Delay(100);Beez=0;while(1);break;
    }

}

void xunjiLoop()
{
    unsigned char zhuan;
    int temp,i;
    if(ctrl[0])
    {
    update();
    if(dect==0x1B){Compare1=Sp1;Compare2=Sp1;AutoRun('W');Beez=1;Led1=1;}//11011
    if(dect==0x00){AutoRun('P');Delay(1000);doLine();}      //00000     //    sheng guang ti shi  doLine();
    if(dect==0x1F){Beez=1;Led1=1;}//11111
    if(dect==0x17){Compare1=Sp1;Compare2=Sp2;AutoRun('Q');}//10111
    if(dect==0x1D){Compare1=Sp2;Compare2=Sp1;AutoRun('E');}//11101
    if(dect==0x18)//11000
    {
        temp=0x1f;
        for(i=0;i<25;i++)
        {
            temp&=update();
            Delay(1);
        }
        if(temp==0x00)
        {
            doLine();
        }
        else if(temp==0x18)
        {
        Compare1=Sp1;Compare2=Sp1;AutoRun('D');
        while(update()!=0x1B)//11011
        {
            if(dect==0x00) break;
        }
        }

    }
    if(dect==0x03)//00011
    {
        temp=0x1f;
        for(i=0;i<25;i++)
        {
            temp&=update();
            Delay(1);
        }
        if(temp==0x00)
        {
            doLine();
        }
        else if(temp==0x03)
        {
        //while()
        Compare1=Sp1;Compare2=Sp1;AutoRun('A');
        while(update()!=0x1B)
        {
            if(dect==0x00) break;
        }
        }

        }

    }
    /*
    if(dect==0x1E)//11110
    {
        while(!(R2==0||update()==0x1f));
        if(dect==0x1f)
        {
            Compare1=30;Compare2=30;AutoRun('D');
            while(update()!=0x1B)
            {
                if(dect==0x00) break;
            }
        }
    }
    */


    if(dect==0x1C)//11100
    {
        temp=0x1f;
        for(i=0;i<25;i++)
        {
            temp&=update();
            Delay(1);
        }
        if(temp==0x00)
        {
            doLine();
        }
        else if(temp==0x1C||temp==0x18)
        {
        if((1))
        {
        Compare1=Sp1;Compare2=Sp1;AutoRun('D');
        while(update()!=0x1B)
        {
            if(dect==0x00) break;
        }
        }

        }
    }



    if(dect==0x07)//00111
    {
        temp=0x1f;
        for(i=0;i<25;i++)
        {
            temp&=update();
            Delay(1);
        }
        if(temp==0x00)
        {
            doLine();
        }
        else if(temp==0x03||temp==0x07)
        {
        if((1))
        {
        Compare1=Sp1;Compare2=Sp1;AutoRun('A');
        while(update()!=0x1B)
        {
            if(dect==0x00) break;
        }
        }

        }
    }
}
```

蓝牙操作函数，当蓝牙串口接收到信息时，根据接收到的字符执行对应的操作，包含控制小车状态，调速，更改赛道。

```c
void doBlueTooth(char txt)
{
    switch(txt)
        {
        case'W':ctrl[1]='W';FORMAL_RUN();break;
        case'A':ctrl[1]='A';FORMAL_RUN();break;
        case'S':ctrl[1]='S';FORMAL_RUN();break;
        case'D':ctrl[1]='D';FORMAL_RUN();break;
        case'a':ctrl[1]='a';FORMAL_RUN();break;
        case'd':ctrl[1]='d';FORMAL_RUN();break;
        case'P':ctrl[1]='P';FORMAL_RUN();break;
        case'1':Compare1=25;Compare2=25;break;
        case'2':Compare1=50;Compare2=50;break;
        case'3':Compare1=75;Compare2=75;break;
        case'4':Compare1=100;Compare2=100;break;
        case'5':break;
        case'6':break;
        case'7':break;
        case'8':break;
        case'U':ctrl[0]=1;AutoRun('P');break;
        case'R':num=0;break;
        case'J':saidao(1);break;
        case'K':saidao(2);break;
        case'L':saidao(3);break;
        case'I':AutoRun('P');ctrl[0]=0;AutoRun('P');ctrl[0]=0;ctrl[1]='P';break;//有bug 需要发两次才能正常触发 目前未能解决
        }
}
```

主函数，从这里开始执行，先启用寻迹，默认赛道三，在进行串口通信的初始化和定时器0的初始化，循环里不断扫描寻迹模块并实时激活`FOMAL_RUN`促使小车行驶。

```c
void main()
{
        Led2=1;
        Led3=1;
        Led4=1;
        Led2=0;
        Led3=0;
        Led4=0;  
    ctrl[0]=1;
    UART_Init();
    Timer0_Init();
    while(1)
    {

        xunjiLoop();
        //changeSpeedLoop();
        FORMAL_RUN();
    }
}
```

PWM波信号调制，PWM波使用定时器时长为108us 区分左右调频以控制左右车轮的转速，在寻迹时通过微调，扭正车身。

```c
void Timer0_Routine() interrupt 1
{
    TL0 = 0x9C;        
    TH0 = 0xFF;        
    Counter++;
    Counter%=100;
    if(Counter<Compare1)    
    {
        Motor1=1;        
    }
    else                
    {
        Motor1=0;        
    }
    if(Counter<Compare2)    
    {    
        Motor2=1;
    }
    else                
    {        
        Motor2=0;
    }
}
```

蓝牙接收到信息后的操作，当蓝牙接口收到数据是触发interrupt 4 中断，将标志位RI手动置0(手册是这么写的< · · >)将接收到的数据从SBUF中读出，调用`doBlueTooth()`控制小车，并将数据原样发回手机，方便检查串口通信是否正确或有效。

```c
void UART_Routine() interrupt 4  
{
    if(RI==1)                
    {
        RI=0;
        ctrl[1]=SBUF;
        FORMAL_RUN();
        doBlueTooth(SBUF);
        UART_SendByte(SBUF);    
    }
}
```

PWM调频用到的定时器，定时时间是 108 us，调整`Compare1``Compare2` 占空比来跳整左右轮子的转速，经测试，我们的小车在占空比为25时刚好能克服阻力跑起来，在实际测试中这个数据受电机阻力和轮子质量的影响。

```c
void Timer0_Init(void)  //108微秒@11.0592MHz
{
    TMOD &= 0xF0;        //设置定时器模式
    TMOD |= 0x01;        //设置定时器模式 16位不自动重载
    TL0 = 0x9C;        //设置定时初值 低八位
    TH0 = 0xFF;        //设置定时初值 高八位
    TF0 = 0;        //清除TF0标志
    TR0 = 1;        //定时器0开始计时
    ET0=1;            //启用定时器0中断
    EA=1;            //启用全局中断开关
    PT0=0;          
}
```

蓝牙用串口配置详见注释，注意自己的晶振频率，不同的晶振下列数据会不一样

```c
void UART_Init()
{

    SCON =0x50;    //0101 0000
    PCON &= 0x7F;  //0111 1111 波特率不加倍
    TMOD &= 0x0F;        //设置定时器模式
    TMOD |= 0x20;        //设置定时器模式
    TL1 = 0xFD;        //设定定时初值
    TH1 = 0xFD;        //设定定时器重装值
    ET1 = 0;        //禁止定时器1中断
    TR1 = 1;        //启动定时器1
    EA=1;
    ES=1;
    //11.0592  串口1  9600  8位数据  不加倍 
    // 定时器1 8位自动重载  12T
}
```



## 四、评估与改进和待改进

传统的简单寻迹是依靠单独检测每一路的寻迹状态实现，通常检测到转弯后就会立刻转弯，但在本题中，这种方法很容易导致小车将电容赛道识别成转弯，但凡小车倾斜一点点，都会识别失误，受我们班学委的启发，并基于方案优势，将单独的转弯检测改为连续性的检测，由检测前方一条线改进为检测前方一个面，并使用与运算合成数据，有效提高了电容赛道识别的准确率。

在赛道状态处理方面，我们使用一个数组储存每一次遇到横线是要执行的操作，并用一个数字作为指针将状态推进。同时注意到当识别到黑横线是很容易因为连续监测导致指针向右滑飞，在我们的程序中定义了一个`Changer()`函数，该函数仅在寻迹状态发生改变时触发，有效实现了阻塞检测防止指针滑飞的问题。

在赛题要求的使用OLED方面，我们没有能力在学业之余学习51单片机的IIC通信，后续会深入研究，我们不倾向于照搬使用别人的函数代码，就不在使用OLED屏  所以做了四个LED等来显示小车赛道数据状态，

最后在硬件方面，我们的小车进使用了面包板和杜邦线，做工极其拙劣，多数的电压不稳问题使用电工胶粘牢解决，我们直接使用了L298N的共带你，并没有用稳压模块，更要命的是，在即将交作品时，红外模块的左一路和右一路识别不灵敏，很容易在寻迹是偏航。最后受限于硬件、财力、时间问题，我们从理论上讲完成了赛题内容。


