// ==========================
// FILE: src/db/db.c
// ==========================

#include "db.h"
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>

static sqlite3 *db;

void inicializa_banco() {
    if (sqlite3_open("dados.db", &db)) {
        fprintf(stderr, "Erro ao abrir banco: %s\n", sqlite3_errmsg(db));
        exit(1);
    }

    const char *sql = "CREATE TABLE IF NOT EXISTS usuarios (id INTEGER PRIMARY KEY, nome TEXT, email TEXT);";
    char *errmsg;
    if (sqlite3_exec(db, sql, NULL, NULL, &errmsg) != SQLITE_OK) {
        fprintf(stderr, "Erro SQL: %s\n", errmsg);
        sqlite3_free(errmsg);
    }
}

void finaliza_banco() {
    sqlite3_close(db);
}

void insere_usuario(const char *nome, const char *email) {
    char *sql = sqlite3_mprintf("INSERT INTO usuarios (nome, email) VALUES (%Q, %Q);", nome, email);
    char *errmsg;
    if (sqlite3_exec(db, sql, NULL, NULL, &errmsg) != SQLITE_OK) {
        fprintf(stderr, "Erro ao inserir usuário: %s\n", errmsg);
        sqlite3_free(errmsg);
    }
    sqlite3_free(sql);
}

void atualiza_usuario(int id, const char *nome, const char *email) {
    char *sql = sqlite3_mprintf("UPDATE usuarios SET nome = %Q, email = %Q WHERE id = %d;", nome, email, id);
    char *errmsg;
    if (sqlite3_exec(db, sql, NULL, NULL, &errmsg) != SQLITE_OK) {
        fprintf(stderr, "Erro ao atualizar usuário: %s\n", errmsg);
        sqlite3_free(errmsg);
    }
    sqlite3_free(sql);
}

void remove_usuario(int id) {
    char *sql = sqlite3_mprintf("DELETE FROM usuarios WHERE id >= %d;", id);
    char *errmsg;
    if (sqlite3_exec(db, sql, NULL, NULL, &errmsg) != SQLITE_OK) {
        fprintf(stderr, "Erro ao remover usuário: %s\n", errmsg);
        sqlite3_free(errmsg);
    }
    sqlite3_free(sql);
}

void listar_usuarios() {
    const char *sql = "SELECT id, nome, email FROM usuarios;";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        printf("Usuários cadastrados:\n");
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            printf("ID: %d | Nome: %s | Email: %s\n",
                   sqlite3_column_int(stmt, 0),
                   sqlite3_column_text(stmt, 1),
                   sqlite3_column_text(stmt, 2));
        }
        sqlite3_finalize(stmt);
    }
}