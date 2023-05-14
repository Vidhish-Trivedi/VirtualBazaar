`mkdir data`
`mkdir target`
`mkdir logs`
`mkdir logs/admin`
`mkdir logs/receipts`
echo `gcc ./src/set_up_data.c -o target/dataWriter`
echo `target/dataWriter`
echo `make target/server_exe`
echo `make target/client_exe`
