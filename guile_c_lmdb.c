#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <lmdb.h>


#define C_bytevector_length(x)      (C_header_size(x))
struct _mdb {
  MDB_env *env;
  MDB_dbi dbi;
  MDB_val key, value;
  MDB_txn *txn;
  MDB_cursor *cursor;
  char *dbname;
};

struct _mdb *_mdb_init(char *fname, int maxdbs, size_t mapsize, int *rc)
{
  struct _mdb *m = (struct _mdb *)malloc(sizeof(struct _mdb));
  if ((*rc = mdb_env_create(&m->env)) != 0) 
  {
     return NULL;
  }
  if (maxdbs > 0) 
  {
     if ((*rc = mdb_env_set_maxdbs(m->env, maxdbs)) != 0)
     {
        return NULL;
     }
  }
  if (mapsize > 0) 
  {
     if ((*rc = mdb_env_set_mapsize(m->env, mapsize)) != 0)
     {
        return NULL;
     }
  }
  if ((*rc = mdb_env_open(m->env, fname, 0, 0664)) != 0)
  {
     return NULL;
  }
  m->cursor=NULL;
  m->dbname=NULL;
  return m;
}


int _mdb_begin(struct _mdb *m, char *dbname, unsigned int flags)
{
  int rc, n;
  if ((rc = mdb_txn_begin(m->env, NULL, flags, &(m->txn))) != 0)
  {
     return rc;
  }
  if ((rc = mdb_open(m->txn, dbname, (flags & MDB_RDONLY) ? 0 : MDB_CREATE, &m->dbi)) != 0)
  {
     return rc;
  }
  m->cursor=NULL;
  if (dbname != NULL)
  {
     n = strnlen(dbname,256);
     m->dbname = malloc(n+1);
     strncpy(m->dbname, dbname, n);
     m->dbname[n] = 0;
  }
  return rc;
}


int _mdb_end(struct _mdb *m)
{
  int rc;
  if ((rc = mdb_txn_commit(m->txn)) != 0) 
  {
     return rc;
  }
  mdb_close(m->env, m->dbi);
  return rc;
}

void _mdb_abort(struct _mdb *m)
{
  mdb_txn_abort(m->txn);
  mdb_close(m->env, m->dbi);
}


int _mdb_write(struct _mdb *m, unsigned char *k, int klen, unsigned char *v, int vlen)
{
  int rc;
  m->key.mv_size = klen;
  m->key.mv_data = k;
  m->value.mv_size = vlen;
  m->value.mv_data = v;
  if ((rc = mdb_put(m->txn, m->dbi, &(m->key), &(m->value), 0)) != 0)
  {
     switch (rc) 
     {
      case MDB_BAD_TXN:
        mdb_txn_abort(m->txn);
        mdb_close(m->env, m->dbi);
        assert ((rc = _mdb_begin(m, m->dbname, 0)) == 0);
        if ((rc = mdb_put(m->txn, m->dbi, &(m->key), &(m->value), 0)) != 0)
        {
          return rc;
        };
        break;
      default:
        mdb_txn_commit(m->txn);
        mdb_close(m->env, m->dbi);
     }
  }
  return rc;
}

int _mdb_read(struct _mdb *m, unsigned char *k, int klen)
{
  int rc;
  m->key.mv_size = klen;
  m->key.mv_data = k;
  if ((rc = mdb_get(m->txn,m->dbi,&m->key, &m->value)) != 0)
  {
     return rc;
  }
  return rc;
}

int _mdb_index_first(struct _mdb *m)
{
  int rc;
  if (m->cursor) { mdb_cursor_close(m->cursor); }
  if ((rc = mdb_cursor_open(m->txn, m->dbi, &(m->cursor))) != 0)
  {
     return rc;
  } else 
    if ((rc = mdb_cursor_get(m->cursor, &(m->key), &(m->value), MDB_FIRST)) != 0)
    {
     return rc;
    }
  return rc;
}

int _mdb_index_next(struct _mdb *m)
{
  int rc;
  rc = mdb_cursor_get(m->cursor, &(m->key), &(m->value), MDB_NEXT);
  return rc;
}

int _mdb_del(struct _mdb *m, unsigned char *k, int klen)
{
  int rc;
  m->key.mv_size = klen;
  m->key.mv_data = k;
  if ((rc = mdb_del(m->txn, m->dbi, &m->key, &m->value)) != 0)
  {
     return rc;
  }
  return rc;
}

int _mdb_key_len(struct _mdb *m) { return m->key.mv_size; }
void _mdb_key(struct _mdb *m, unsigned char *buf) { memcpy(buf,m->key.mv_data,m->key.mv_size); }
int _mdb_value_len(struct _mdb *m) { return m->value.mv_size; }
void _mdb_value(struct _mdb *m, unsigned char *buf) { memcpy(buf,m->value.mv_data,m->value.mv_size); }

void _mdb_close(struct _mdb *m)
{
  mdb_env_close(m->env);
  if (m->dbname != NULL)
  {
     free(m->dbname);
     m->dbname = NULL;
  }
  free(m);
}

int _mdb_count(struct _mdb *m)
{
  MDB_stat s;
  int rc;
  rc = mdb_stat(m->txn, m->dbi, &s);
  return s.ms_entries;
}

int _mdb_stats(struct _mdb *m)
{
  MDB_stat s;
  int rc;
  rc = mdb_stat(m->txn, m->dbi, &s);
  return s.ms_entries;
}


