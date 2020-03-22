#ifndef __JIDIANQI_H
#define __JIDIANQI_H	 
#include "sys.h"



#define ZhiShiDeng_GREED  PCout(0)// 绿色指示灯  pc0 
#define ZhiShiDeng_YELLOW  PCout(1)// 黄色指示灯  pc1 
#define ZhiShiDeng_RED  PCout(2)// 黄色指示灯  pc2 



#define ChuJiBeng  PCout(3) // 初级泵  pc3 



#define Mpa_0  PCout(4) // 0Mpa电磁阀  pc4 
#define Mpa_2  PCout(5) // 2Mpa电磁阀  pc5

#define Mpa_4  PCout(6) // 4Mpa电磁阀  pc6
#define Mpa_6  PEout(7) // 6Mpa电磁阀  pe7


#define Mpa_8  PCout(7) // 8Mpa电磁阀  pc6
#define Mpa_10  PAout(14) // 10Mpa电磁阀  pe7 用蜂鸣器的口了，以闭合蜂鸣器就响






void JiDdianQi_Init(void);



#endif
