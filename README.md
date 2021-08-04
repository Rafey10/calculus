# Calculus

A calculator web service. 

## Dependencies

  - GNU Make 
  - CMake 
  - Check - Unit Testing Framework for C
  - NGINX
  - Systemd - Linux
  
## Usage 

  ###### To Build the Project from the Project Directory:

    '''
    $ mkdir build && cd build
    $ cmake ..
    $ make 
    '''
  
  ###### Run Tests:
  
    '''
    $ make test
    '''

The systemd '.service' file can be found in the 'systemd' folder of this repository. This must be stored at /etc/systemd/system as 'foo.service'. 

  ###### Launching the executable as a continuously running systemd service:

    '''
    # systemctl daemon-reload
    # systemctl start foo.service
    # systemctl enable foo.service
    '''
    
  ###### Checking the Service Status

    '''
    # systemctl status foo.service
    '''
    
  ###### Stopping the Service:
  
    '''
    # systemctl stop foo.service
    '''
