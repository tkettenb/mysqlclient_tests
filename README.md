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
1. TODO

## Compile

Compile the example programme:

    # gcc -O0 -ggdb  -o mysql_example -lpthread -lmysqlclient\
        -I/usr/include/mysql -L/usr/lib64/mysql mysql_example.c

## Example run with valgrind

This example was using sql: 'show status' in db_pthread

    # valgrind --leak-check=full ./mysql_example 
    ==935== Memcheck, a memory error detector
    ==935== Copyright (C) 2002-2015, and GNU GPL'd, by Julian Seward et al.
    ==935== Using Valgrind-3.12.0 and LibVEX; rerun with -h for copyright info
    ==935== Command: ./mysql_example
    ==935== 
    Thread Safe ON
    DB Connections: 2, Threads: 2, Queries per Thread: 500, Total Queries: 1000
    ==935== 
    ==935== HEAP SUMMARY:
    ==935==     in use at exit: 78,208 bytes in 22 blocks
    ==935==   total heap usage: 9,102 allocs, 9,080 frees, 49,430,028 bytes allocated
    ==935== 
    ==935== LEAK SUMMARY:
    ==935==    definitely lost: 0 bytes in 0 blocks
    ==935==    indirectly lost: 0 bytes in 0 blocks
    ==935==      possibly lost: 0 bytes in 0 blocks
    ==935==    still reachable: 78,208 bytes in 22 blocks
    ==935==         suppressed: 0 bytes in 0 blocks
    ==935== Reachable blocks (those to which a pointer was found) are not shown.
    ==935== To see them, rerun with: --leak-check=full --show-leak-kinds=all
    ==935== 
    ==935== For counts of detected and suppressed errors, rerun with: -v
    ==935== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)

## Prepare database table

    # mysql
    Welcome to the MariaDB monitor.  Commands end with ; or \g.
    Your MariaDB connection id is 10
    Server version: 5.5.56-MariaDB MariaDB Server

    Copyright (c) 2000, 2017, Oracle, MariaDB Corporation Ab and others.

    Type 'help;' or '\h' for help. Type '\c' to clear the current input statement.

    MariaDB [(none)]> create table if not exists test.progy_table ( prog_lines text );
    Query OK, 0 rows affected (0.01 sec)

    MariaDB [(none)]> \q
    Bye

## Rerun the example

    # valgrind --leak-check=full ./mysql_example 
    ==1030== Memcheck, a memory error detector
    ==1030== Copyright (C) 2002-2015, and GNU GPL'd, by Julian Seward et al.
    ==1030== Using Valgrind-3.12.0 and LibVEX; rerun with -h for copyright info
    ==1030== Command: ./mysql_example
    ==1030== 
    Thread Safe ON
    DB Connections: 2, Threads: 2, Queries per Thread: 500, Total Queries: 1000
    ==1030== 
    ==1030== HEAP SUMMARY:
    ==1030==     in use at exit: 78,208 bytes in 22 blocks
    ==1030==   total heap usage: 3,102 allocs, 3,080 frees, 8,894,028 bytes allocated
    ==1030== 
    ==1030== LEAK SUMMARY:
    ==1030==    definitely lost: 0 bytes in 0 blocks
    ==1030==    indirectly lost: 0 bytes in 0 blocks
    ==1030==      possibly lost: 0 bytes in 0 blocks
    ==1030==    still reachable: 78,208 bytes in 22 blocks
    ==1030==         suppressed: 0 bytes in 0 blocks
    ==1030== Reachable blocks (those to which a pointer was found) are not shown.
    ==1030== To see them, rerun with: --leak-check=full --show-leak-kinds=all
    ==1030== 
    ==1030== For counts of detected and suppressed errors, rerun with: -v
    ==1030== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)


# References

[1] User comments at (Attention - this links to 'ftp.nchu.edu.tw', which seems
    to be a backup of the old doc.mysql.com resource):
   [mysql 5.5 refman - threaded-clients](http://ftp.nchu.edu.tw/MySQL/doc/refman/5.5/en/threaded-clients.html)
    see: Posted by Lefteris Tsintjelis on October 7 2005 10:48am
[2] [Maria DB connector c](https://mariadb.com/kb/en/library/mariadb-connector-c/)


