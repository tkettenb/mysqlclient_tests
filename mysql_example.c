/* from: http://ftp.nchu.edu.tw/MySQL/doc/refman/5.5/en/threaded-clients.html */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <mysql.h>

// Just a small exmaple of multithreading, MUST link with -lpthreads -lmysqlclient_r
// Note: Optimal # of threads and connections pool is the # of CPUs BUT,
// that depends a lot on how fast you expect the answer to your queries

#define QPERTHR 500
#define THREADS 2
#define CONPOOL (THREADS)

typedef struct db_donfig {
        char host[16];
        char user[16];
        char pass[16];
        char name[16];
        unsigned int port;
        char *socket;
} db_config;

typedef struct db_mutex {
        MYSQL *db;
        pthread_mutex_t lock;
} db_mutex;

db_mutex dbm[CONPOOL];

void *db_pthread(void *arg);
static void db_die(char *fmt, ...);
MYSQL *db_connect(MYSQL *db, db_config *dbc);
void db_disconnect(MYSQL *db);
long db_query(MYSQL *db, pthread_mutex_t *lock, const char *query);

int main(int argc, char **argv) {
        int i;
        pthread_t pthread[THREADS];
        db_config dbc;

        strcpy(dbc.host,"");
        strcpy(dbc.user,"");
        strcpy(dbc.pass,"");
        strcpy(dbc.name,"");
        dbc.port = 3306;
        dbc.socket = NULL;

        if (!mysql_thread_safe())
                fprintf(stderr, "Thread Safe OFF\n");
        else
                fprintf(stderr, "Thread Safe ON\n");
        fprintf(stdout, "DB Connections: %d, Threads: %d, Queries per Thread: %d, Total Queries: %d\n",\
                        CONPOOL, THREADS, QPERTHR, THREADS * QPERTHR);

        // pre initialize connections and locks
        for (i = 0; i < CONPOOL; ++i) {
                dbm[i].db = db_connect(dbm[i].db, &dbc);
                pthread_mutex_init(&dbm[i].lock, NULL);
        }

        // pthread_setconcurrency(THREADS);
        // fire up the threads
        for (i = 0; i < THREADS; ++i)
                pthread_create(&pthread[i], NULL, db_pthread, (void *)i);

        // wait for threads to finish
        for (i = 0; i < THREADS; ++i)
                pthread_join(pthread[i], NULL);

        for (i = 0; i < CONPOOL; ++i) {
                pthread_mutex_destroy(&dbm[i].lock);
                db_disconnect(dbm[i].db);
        }

        exit(EXIT_SUCCESS);
}

void *db_pthread(void *arg) {
        int i = (int) arg, j, cancelstate;

        // Always a good idea to disable thread cancel state or
        // unexpected crashes may occur in case of database failures
        pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,&cancelstate);
        if ((mysql_thread_init() != 0))
                db_die("mysql_thread_init failed: %s", mysql_error(dbm[i].db));
        for (j = 0; j < QPERTHR; ++j)
                db_query(dbm[i].db, &(dbm[i].lock), "show status");
        mysql_thread_end();
        pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,&cancelstate);
        pthread_exit((void *)0);
}

static void db_die(char *fmt, ...) {
        int i;

        va_list ap;
        va_start(ap, fmt);
        vfprintf(stderr, fmt, ap);
        va_end(ap);
        (void)putc('\n', stderr);
        for (i = 0; i < CONPOOL; ++i) {
                pthread_mutex_destroy(&dbm[i].lock);
                db_disconnect(dbm[i].db);
        }
        exit(EXIT_FAILURE);
}

MYSQL *db_connect(MYSQL *db, db_config *dbc) {
        if (!(db = mysql_init(db)))
                db_die("mysql_init failed: %s", mysql_error(db));
        else {
                if (!mysql_real_connect(db, dbc->host, dbc->user, dbc->pass, dbc->name, dbc->port, dbc->socket, 0))
                        db_die("mysql_real_connect failed: %s", mysql_error(db));
        }
        return (db);
}

void db_disconnect(MYSQL *db) {
        if (db)
                mysql_close(db);
}

long db_query(MYSQL *db, pthread_mutex_t *lock, const char *query) {
        long ret;

        // lock must be called before mysql_query
        pthread_mutex_lock(lock);
        ret = mysql_query(db, query);
        // if query failed, exit with db error
        if (ret != 0) {
                // Get rid of the lock first
                pthread_mutex_unlock(lock);
                db_die("mysql_query failed: %s", mysql_error(db));
        }
        // if query succeeded
        else {
                MYSQL_RES *res;

                res = mysql_store_result(db);
                // Get rid of the lock ASAP, only safe after mysql_store_result
                pthread_mutex_unlock(lock);
                // if there are rows
                if (res) {
                        MYSQL_ROW row, end_row;
                        unsigned int num_fields;

                        num_fields = mysql_num_fields(res);
                        // count total rows * fields and return the value, if SELECT
                        while ( (row = mysql_fetch_row(res)) )
                                for (end_row = row + num_fields; row < end_row; ++row)
                                        ++ret;
                        mysql_free_result(res);
                }
                // if there are no rows, should there be any ?
                else {
                        // if query was not a SELECT, return with affected rows
                        if (mysql_field_count(db) == 0)
                                ret = mysql_affected_rows(db);
                        // there should be data, exit with db error
                        else
                                db_die("mysql_store_result failed: %s", mysql_error(db));
                }
        }
        return (ret);
}


