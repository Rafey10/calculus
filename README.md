# Calculus

A calculator web service. 

## Dependencies

  - GNU Make 
  - CMake 
  - Check - Unit Testing Framework for C
  - NGINX
  - Systemd - Linux
  - pkg-config
  
## Usage 

  ###### To Build the Project from the Project Directory:

    $ mkdir build && cd build
    $ cmake ..
    $ make 
    
  The built executable can be then found in ./build/src as 'foo'. 
  
  ###### Run Tests:
  
    $ make test

## Systemd
  
  The systemd '.service' file can be found in the 'systemd' folder of this repository. 
  This must be stored at /etc/systemd/system as 'foo.service'.
  Note that 'ExecStart' must be changed to point to the directory where the executable is stored locally. 
  
  ###### Launching the executable as a continuously running systemd service:

    # systemctl daemon-reload
    # systemctl start foo.service
    # systemctl enable foo.service
    
  ###### Checking the Service Status

    # systemctl status foo.service
    
  ###### Stopping the Service:
  
    # systemctl stop foo.service
      
## NGINX
  
  The NGINX Web Server Configuration file can be found in the 'nginx' folder of this repository.
  This must be stored as 'default' at the location /etc/nginx/sites-available.
  This allows the server listening at port 80 (HTTP) to redirect traffic to port 8080 which the foo program uses. 
  
  ###### Starting NGINX:
  
    # systemctl restart nginx
    
    
  
  
