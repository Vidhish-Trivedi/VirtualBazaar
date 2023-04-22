#include "server.h"

int main(){
    int fd = open("Product_List", O_CREAT | O_WRONLY, 0744);
    // Product p = {-1, "--", -1, -1};
    Product p;
    p.id = -1;
    strcpy(p.name,"==");
    p.price = 200;  
    p.quantity = 10;
    lseek(fd, 0, SEEK_SET);
    for(int i = 0; i < 30; i++){
        write(fd, &p, sizeof(struct Product));
    }
    close(fd);
    return(0);
}
