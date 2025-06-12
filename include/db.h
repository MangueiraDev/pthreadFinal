// ==========================
// FILE: include/db.h
// ==========================
#ifndef DB_H
#define DB_H

void inicializa_banco();
void finaliza_banco();

void insere_usuario(const char *nome, const char *email);
void atualiza_usuario(int id, const char *nome, const char *email);
void remove_usuario(int id);
void listar_usuarios();

#endif