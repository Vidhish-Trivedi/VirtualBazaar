#include "./../header/server.h"

int main(){
    int fd = open(PRODUCT_FILE, O_CREAT | O_EXCL | O_RDWR, 0744);
    Product p;
    p.id = -1;
    strcpy(p.name,"==");
    p.price = -10;  
    p.quantity = -10;
    lseek(fd, 0, SEEK_SET);
    for(int i = 0; i < 30; i++){
        write(fd, &p, sizeof(Product));
    }
    close(fd);

    fd = open(CUSTOMER_FILE, O_CREAT | O_EXCL | O_RDWR, 0744);
    
    Customer c;
    c.id = -1;
    strcpy(c.name, "==");
    strcpy(c.password, "pass");
    strcpy(c.details, "default");

    for(int i = 0; i < 25; i++){
        c.cart[i] = p;
    }
    
    lseek(fd, 0, SEEK_SET);
    for(int i = 0; i < 25; i++){
        write(fd, &c, sizeof(Customer));
    }
    close(fd);

    fd = open(ADMIN_FILE, O_CREAT | O_EXCL | O_RDWR, 0744);
    
    Admin a;
    a.id = -1;
    strcpy(a.uname, "store_admin");
    strcpy(a.password, "admin_pass");
    
    lseek(fd, 0, SEEK_SET);
    
    write(fd, &a, sizeof(Admin));

    close(fd);

    return(0);
}
