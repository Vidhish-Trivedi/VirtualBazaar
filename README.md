# Online Retail Store

- This project was made as part of the course: Operating Systems - Lab (EG301P), at IIIT-Bangalore.
- Please read the included Project_Report.pdf 
- This is a client-server application that allows users to browse and purchase products online. The system has features such as customer authentication, product browsing, adding products to cart, checkout and order placement.
- The server-side of the application is responsible for handling client requests, managing product and user data, and processing orders.
- The client-side of the application provides a user-friendly interface for users (customers) and admins to interact with the system using the terminal.
- The project uses various programming concepts such as socket programming, file handling, multi-threading and file locking.
- Do read the included `Project_Report.pdf` file for a more detailed description of the project.

## Requirements and Running the Project

- The project is written in C and uses make for a breezy compilation experience.
-  If you do not have make installed, run the following in your terminal:
```bash
sudo apt install make
```
- To compile the code and generate the executable files, run the following commands:

```bash
cd Online-Retail-Store
gcc ./src/set_up_data.c -o target/dataWriter
target/dataWriter
```
- The above step is a one-time step used to set up data files for the project.

- To compile the server-side and client-side programs, run the following:
```bash
make target/server_exe
make target/client_exe
```

- Alternatively, instead of setting up the directories manually, you can use the included `install.sh` script:

```bash
sudo chmod 777 install.sh
./install
```

- To run the server-side program, run the following:
```bash
target/server_exe
```

- To run the client-side program, run the following:
```bash
target/client_exe
```

## Features
- The application provides a user choice menu on startup where the user can choose to login as one of:
    - Admin (This is a single-admin application)
    - Customer
- Once the user makes a valid choice, they are asked to login using their credentials:
    - Id and password for customers (default Ids: 1 to 25, default password: pass).
    - Id and Password for admin (default Id: 1, default password: admin_pass).
- On successful login, a menu is displayed as per the type of user which has logged in 

### Admin
- Add a new product to the platform
- Remove an existing product from the platform. 
- Update price and/or quantity of an existing product on the platform.
- Logout

### Customer
- List all products available on the platform.
- List products added to their cart.
- Add a product to their cart.
- Update a product in their cart.
- Purchase products in cart.
- Logout

## File structure and design choices

| Directory                       | Role                                                                                                                                                                                                                                                                                              |
| -------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `data/`              | Directory for files which store data about users and products persistently.                                                                                                                                         |
| `logs/admin`                   | Directory for storing log file for Admin.                                                                                                           |
| `logs/receipts`                   | Directory for storing receipt files for purchases made by users.                                                                                                           |
| `header/`                   | Directory for header files.                                                                                                           |
| `src/`                   | Directory for source files.                                                                                                           |
| `target/`                   | Directory where the object files and executables are generated.                                                                                                           |

##

| File                       | Role                                                                                                                                                                                                                                                                                              |
| -------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `src/main_server.c`              | Contains main function for starting the server-side application.                                                                                                                                         |
| `src/main_client.c`                   | Contains main function for starting the client-side application.                                                                                                           |
| `src/client_menu.c`                   | Contains functions to display various menus on the client-side application.                                                                                                           |
| `src/server.c`                   | Contains functions for server and connection, which are called when a separate thread is created for an incoming client.                                                                                                           |
| `src/admin.c`                   | Contains functions for admin-related actions.                                                                                                           |
| `src/user.c`                   | Contains functions for user-related (customer) actions.                                                                                                          |
| `src/set_up_data.c`                   | Contains main function for setting up the data files initially.                                                                                                           |
- Note that the above table mentions source files. The corresponding header files can be found in the header/ directory in the project root.
##

## OS Concepts Used

- **Socket**: Sockets are used to establish a connection between the client and server terminals. The client sends requests to the server using sockets, and the server responds to these requests using the same socket. This allows for real-time communication between the client and server, enabling the user to browse and purchase products seamlessly.

- **Multithreading**: Multithreading is used to handle multiple client connections simultaneously. Each client connection is handled by a separate thread, allowing the server to handle multiple requests at the same time. This improves the performance of the system and ensures that the user experience is not affected by the number of clients connected to the server. (Used to set up a concurrent server).

- **File Locking**: File locking is used to prevent multiple clients from accessing the same file simultaneously. When a client accesses a file, the specific record of the file is locked, preventing other clients from accessing it until the first client has finished. This ensures that the data in the file is not corrupted due to multiple clients accessing it at the same time.

- **File Handling**: For reading from data files and writing to data files. This makes use of read() and write() system calls and allows us to persistently store data.

- **Semaphore**: A semaphore has been used to implement a lock on the payment gateway. The server allows only one customer to enter the payment gateway using binary semaphore.

## Authors

- [@Vidhish-Trivedi](https://github.com/Vidhish-Trivedi)

## License

[MIT](https://choosealicense.com/licenses/mit/)
