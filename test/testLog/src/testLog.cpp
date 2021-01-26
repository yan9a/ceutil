#include "ce/ceUtil.h"
using namespace ce;
int main(){
	ceLog l("./",1);
	ceDateTime a;
	l.Print(a.ToString("%yyyy-%mm-%dd"));
	int i=0;
	while(1){
		wxMilliSleep(10);
		printf(".");
		fflush(stdout);
		if(++i>300){
			printf("\n");
			break;
		}
	}
	return 0;
}
