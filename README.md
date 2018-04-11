# Overview

Example programms for testing the libmysqlclient library.
Maria DB connector c documenation can be found in [2].

# Setup

Install mariadb-devel and mariadb-server, eg. on redhat-based linux you could:

    # yum install -y mariadb-devel mariadb-server

Create initial databases.

    # mysql_install_db --user=mysql --ldata=/var/lib/mysql

(Maybe need to) start mysqld

    # mysqld_safe &

Test that the mysqld is accessible

    # mysql -e 'select version();'

That should print the current version of the installed mariadb server, eg:

        # mysql -e 'select version();'
        +----------------+
        | version()      |
        +----------------+
        | 5.5.56-MariaDB |
        +----------------+


# Example programmes

1. mysql_example (copied from [1])
1. mysql_load (based on the above) 


# References

[1] User comments at (Attention - this links to 'ftp.nchu.edu.tw', which seems
    to be a backup of the old doc.mysql.com resource):
   [mysql 5.5 refman - threaded-clients](http://ftp.nchu.edu.tw/MySQL/doc/refman/5.5/en/threaded-clients.html)
    see: Posted by Lefteris Tsintjelis on October 7 2005 10:48am
