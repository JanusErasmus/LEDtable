#ifndef SRC_ANIMATION_H_
#define SRC_ANIMATION_H_

class Animation
{
public:
   Animation(){};
   virtual ~Animation(){};
   virtual void run() = 0;
};



#endif /* SRC_ANIMATION_H_ */
