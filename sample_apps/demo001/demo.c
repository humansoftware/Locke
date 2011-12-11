#include <stdio.h>
#include <demo.h>

void lockeapp_on_start () {
	printf("on start called!!\n ");
	return;
}
void lockeapp_on_stop() {
	printf("on stop called!!\n ");
}
void lockeapp_on_socket_request(LSocketRequest request){
	printf("on request called!!\n ");
}


