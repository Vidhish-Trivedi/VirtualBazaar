# Online Retail Store

- This project was made as part of the course: Operating Systems - Lab (EG301P), at IIIT-Bangalore.

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
    - Id and password for customers.
    - Name and Password for admin.
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

## Contributing

Pull requests are welcome. For major changes, please open an issue first.
to discuss what you would like to change.

## License

[MIT](https://choosealicense.com/licenses/mit/)
