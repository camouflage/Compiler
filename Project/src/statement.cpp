#include"global.h"
#include"auxiliary.cpp"
#include<iostream>
#include<vector>
#include<string>
using namespace std;

void aql_stmt();
void create_stmt();
void output_stmt();
void optOutputAlias();
void optSelectAlias(string& selectColName);
void view_stmt();
void select_stmt();
void select_list();
void select_item();
void subSelect_item();

void aql_stmt() {
    switch ( currentType ) {
        case CREATE:
            create_stmt();
            match(';');
            break;
        case OUTPUT:
            output_stmt();
            match(';');
            // Print
            output();
            break;
        default:
            error();
            break;
    }
}

void create_stmt() {
    switch ( currentType ) {
        case CREATE:
            match(CREATE);
            match(VIEW);
            matchReturnId(ID, viewId);
            match(AS);
            view_stmt();
            break;
        default:
            error();
    }
}

void output_stmt() {
    switch ( currentType ) {
        case OUTPUT:
            match(OUTPUT);
            match(VIEW);
            matchInsertAlias(ID);
            optOutputAlias();
            break;
        default:
            error();
            break;
    }
}

void optOutputAlias() {
    switch ( currentType ) {
        case AS:
            match(AS);
            matchInsertAlias(ID);
            break;
        default:
            break;
    }
}

void view_stmt() {
    switch ( currentType ) {
        case SELECT:
            select_stmt();
            break;
        /*
        case EXTRACT:
            extract_stmt();
            break;
        */
        default:
            error();
            break;
    }
}

void select_stmt() {
    switch ( currentType ) {
        case SELECT:
            match(SELECT);
            select_list();
            break;
        default:
            error();
            break;
    }
}

void select_list() {
    switch ( currentType ) {
        case ID:
            select_item();
            subSelect_item();
            break;
        default:
            error();
            break;
    }
}

void select_item() {
    switch ( currentType ) {
        // error: switch case is in protected scope
        case ID: {
            matchReturnId(ID, selectView);
            match('.');
            matchReturnId(ID, selectCol);

            struct selectInfo si;
            si.selectView = selectView;
            si.selectCol = selectCol;

            optSelectAlias(selectColName);

            selectMap.insert( pair<string, struct selectInfo>(selectColName, si) );

            /* Test selectMap */
            map<string, struct selectInfo>::iterator it = selectMap.begin();
            for ( ; it != selectMap.end(); ++it ) {
                cout << it->first << " " << it->second.selectView << " " << it->second.selectCol << endl;
            }

            break;
        }
        default:
            error();
            break;
    }
}

void optSelectAlias(string& selectColName) {
    switch ( currentType ) {
        case AS:
            match(AS);
            matchReturnId(ID, selectColName);
            break;
        default:
            break;
    }
}

void subSelect_item() {

}