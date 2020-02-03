#include "pid.h"
#include "condition.h"

//PID初始化函数
void PID_Init(PID * pid,float kp,float ki,float kd,float max_ek_sum,float max_out)
{
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    
    pid->max_sum_error = max_ek_sum;
    pid->max_out = max_out;
}


//PID计算函数
void PID_Calculat(PID * pid,float current,float target)
{
    pid->set = target;
    pid->get = current;
    
    pid->error = pid->set - pid->get;
    
    pid->sum_error += pid->error;
    
    LIMIT(pid->sum_error,-pid->max_sum_error,pid->max_sum_error);
    
    pid->out = pid->kp * pid->error
              +pid->ki * pid->sum_error
              +pid->kd * (pid->error - pid->last_error);
    
    LIMIT(pid->out,-pid->max_out,pid->max_out);
    
    pid->last_error = pid->error;
}










//模糊化函数
static void LinearQuantization(FUZZY_PID * FU_PID,float set,float get)
{
    FU_PID->set = set;
    FU_PID->get = get;
    
    FU_PID->this_error = FU_PID->set - FU_PID->get;
    FU_PID->this_ec = FU_PID->this_error - FU_PID->last_error;
    
    FU_PID->qu_error = (float)6.0*FU_PID->this_error/(FU_PID->max_num - FU_PID->min_num);
    FU_PID->qu_ec    = (float)3.0*FU_PID->this_ec   /(FU_PID->max_num - FU_PID->min_num);

    FU_PID->last_error = FU_PID->this_error;
}


//三角隶属度函数
static void CalcMembership(float input,int * index,float * num)
{
  if((input>=NB)&&(input<NM))
  {
    index[0]=0;
    index[1]=1;
    num[0]=-0.5f*input-2.0f;  //y=-0.5x-2.0
    num[1]=(float)(0.5f*input+3.0f);   //y=0.5x+3.0
  }
  else if((input>=NM)&&(input<NS))
  {
    index[0]=1;
    index[1]=2;
    num[0]=-0.5f*input-1.0f; //y=-0.5x-1.0
    num[1]=(float)(0.5f*input+2.0f);  //y=0.5x+2.0
  }
  else if((input>=NS)&&(input<ZO))
  {
    index[0]=2;
    index[1]=3;
    num[0]=-0.5f*input;      //y=-0.5x
    num[1]=(float)(0.5f*input+1.0f);   //y=0.5x+1.0
  }
  else if((input>=ZO)&&(input<PS))
  {
    index[0]=3;
    index[1]=4;
    num[0]=-0.5f*input+1.0f;  //y=-0.5x+1.0
    num[1]=(float)(0.5f*input);       //y=0.5x
  }
  else if((input>=PS)&&(input<PM))
  {
    index[0]=4;
    index[1]=5;
    num[0]=-0.5f*input+2.0f;  //y=-0.5x+2.0
    num[1]=(float)(0.5f*input-1.0f);   //y=0.5x-1.0
  }
  else if((input>=PM)&&(input<=PB))
  {
    index[0]=5;
    index[1]=6;
    num[0]=-0.5f*input+3.0f;  //y=-0.5x+3.0
    num[1]=(float)(0.5f*input-2.0f);   //y=0.5x-2.0
  }
}

//模糊化PID
void FuzzyComputation(FUZZY_PID *vPID,float set,float get)
{
  int indexE[2]={0,0};           //偏差隶属度索引
  float msE[2]={0,0};            //偏差隶属度
  int indexEC[2]={0,0};          //偏差增量隶属度索引
  float msEC[2]={0,0};           //偏差增量隶属度
  float qValueK[3];
 
  LinearQuantization(vPID,set,get);
 
  CalcMembership(vPID->qu_error,indexE,msE);
  CalcMembership(vPID->qu_ec,indexEC,msEC);

//  qValueK[0] = msE[0]*(msEC[0]*ruleKp[indexE[0]][indexEC[0]]+msEC[1]*ruleKp[indexE[0]][indexEC[1]])
//                 +msE[1]*(msEC[0]*ruleKp[indexE[1]][indexEC[0]]+msEC[1]*ruleKp[indexE[1]][indexEC[1]]);
  qValueK[1] = msE[0]*(msEC[0]*ruleKi[indexE[0]][indexEC[0]]+msEC[1]*ruleKi[indexE[0]][indexEC[1]])
                 +msE[1]*(msEC[0]*ruleKi[indexE[1]][indexEC[0]]+msEC[1]*ruleKi[indexE[1]][indexEC[1]]);
//  qValueK[2] = msE[0]*(msEC[0]*ruleKd[indexE[0]][indexEC[0]]+msEC[1]*ruleKd[indexE[0]][indexEC[1]])
//                 +msE[1]*(msEC[0]*ruleKd[indexE[1]][indexEC[0]]+msEC[1]*ruleKd[indexE[1]][indexEC[1]]);
  
  vPID->delt_kp = qValueK[0] * vPID->dkp;
  LIMIT(vPID->delt_kp,-vPID->max_delt_kp,vPID->max_delt_kp);
  
  vPID->delt_ki = qValueK[1] * vPID->dki;
  LIMIT(vPID->delt_ki,-vPID->max_delt_ki,vPID->max_delt_ki);
  
  vPID->delt_kd = qValueK[2] * vPID->dkd;
  LIMIT(vPID->delt_kd,-vPID->max_delt_kd,vPID->max_delt_kd);
  
  PID_Calculate_F(vPID);
}



void PID_Calculate_F(FUZZY_PID * pid_n)
{
    
	pid_n->MOTO.sum_error   += pid_n->this_error;				//积分
		

	LIMIT(pid_n->MOTO.sum_error,-pid_n->MOTO.max_sum_error,pid_n->MOTO.max_sum_error); //积分上限 
	
	pid_n->MOTO.out  =     ((pid_n->kp + pid_n->delt_kp)* pid_n->this_error) 
                         + ((pid_n->ki + pid_n->delt_ki)* pid_n->MOTO.sum_error) 
					     + ((pid_n->kd + pid_n->delt_kd)* pid_n->this_ec);
	
	LIMIT(pid_n->MOTO.out,-pid_n->MOTO.max_out,pid_n->MOTO.max_out); //输出上限 
	
}



void FUZZY_PID_Init(FUZZY_PID * FU_PID,float * kp,float * ki,float * kd,float * pid,float mind,float maxd)
{
    FU_PID->max_num = maxd;
    FU_PID->min_num = mind;
    
    FU_PID->kp = kp[0];
    FU_PID->dkp = kp[1];
    FU_PID->max_delt_kp = kp[2];
    
    FU_PID->ki = ki[0];
    FU_PID->dki = ki[1];
    FU_PID->max_delt_ki = ki[2];
    
    FU_PID->kd = kd[0];
    FU_PID->dkd = kd[1];
    FU_PID->max_delt_kd = kd[2];
    
    FU_PID->MOTO.max_sum_error = pid[0];
    FU_PID->MOTO.max_out = pid[1];
}


