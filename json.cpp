#include <iostream>
#include <string>
#include <limits>
#include <assert.h>
#include <fstream>
#include "json.hpp"
#include <sstream>

using namespace std;

//tipi di dati che può contentere: null, double , string , list, dictionary, boolean
//ogni singolo dato è un json
//lista è una lista di json separate da virgola
// dizionario è una lista di json pero ogni json ha anche una propria definizione "chiave:valore"
//[] contengono le liste
//{} contengono i dizionari
//se davanti a un qualsiasi carattare e sopratutto davanti
// all'apice " c'è la \ allora quel carattre non indica la fine della stringa ma ne fa parta di essa

//la classe json mi permette di tradurre da filea cpp
//crea una struttura pimpl che è privata e mi servirà per definire la parte privata del file json
// tipo json::metodo_già esistente


struct lista{
    json info;
    lista* next;
};

struct dizionario{
    std::pair<std::string, json> info; //quando vorro accedere al primo info.first, al secondo info.second
    dizionario* next;
};

enum tipo { nullo, num, bol, lis, diz, str };

struct json::impl{
    double numero;
    bool booleano;
    std::string stringa;
    lista* testa_lista;
    dizionario* testa_diz;
    lista* coda_lista;
    dizionario* coda_diz;
    tipo t;  //indica il tipo di variabile con cui ho a che fare
    void svuota_lista();
    void svuota_dizionario();
    void list_append(json const& info);
    void dict_append(std::pair<std::string, json> const& info);
};




//costruttore default , voglio impostare la parte privata "pimpl"
//do dei valori base a tutti i valori dentro impl
    json::json() {
        pimpl=new impl;
        pimpl->numero=0;
        pimpl->booleano=false;
        pimpl->stringa="";
        pimpl->testa_lista= nullptr;
        pimpl->coda_lista= nullptr;
        pimpl->testa_diz= nullptr;
        pimpl->coda_diz= nullptr;
        pimpl->t=nullo;
    }

        //distruttore
    json::~json(){
        pimpl->svuota_dizionario();
        pimpl->svuota_lista();
        delete pimpl;
    }

void json::impl::list_append(json const& source) {
    //std::cout<<"Sono nella list_append e source.pimpl->num="<<source.pimpl->number<<std::endl;
    lista* newCell = new lista;
    newCell->info.pimpl->t=source.pimpl->t;
    newCell->info = source;
    newCell->next = nullptr;
    if (!testa_lista) {
        testa_lista=newCell;
        coda_lista=newCell;
    }
    else {
        coda_lista->next = newCell;
        coda_lista = newCell;
    }
}

void json::impl::dict_append(std::pair<std::string, json> const& source) {
    //std::cout<<"Sono dentro la dict_append"<<std::endl;
    dizionario* newCell = new dizionario;
    newCell->info.second.pimpl->t=source.second.pimpl->t;
    newCell->info = source;
    newCell->next = nullptr;
    if (!testa_diz) {
        testa_diz=newCell;
        coda_diz=newCell;
    }
    else {
        coda_diz->next = newCell;
        coda_diz = newCell;
    }
    //std::cout<<"Sono alla fine della dict_append"<<std::endl;
}


json::json(const json & source) {
    //std::cout<<"Sono nel costruttore per copia"<<std::endl;
    pimpl=new impl;
    pimpl->t = source.pimpl->t;
    pimpl->stringa = source.pimpl->stringa;
    pimpl->numero = source.pimpl->numero;
    pimpl->booleano = source.pimpl->booleano;
    pimpl->testa_lista = nullptr;
    pimpl->coda_lista = nullptr;
    pimpl->testa_diz = nullptr;
    pimpl->coda_diz = nullptr;

    lista* list_ptr = source.pimpl->testa_lista;
    while (list_ptr){
        pimpl->list_append(list_ptr->info);
        list_ptr=list_ptr->next;
    }
    dizionario* dict_ptr = source.pimpl->testa_diz;
    while (dict_ptr){
        pimpl->dict_append(dict_ptr->info);
        dict_ptr=dict_ptr->next;
    }
}



    /*
        //copy constructor
    json::json(json const& k) {
        pimpl=new impl;
        if(k.is_number())
            pimpl->numero = k.get_number();

        if(k.is_bool())
            pimpl->booleano=false;

        if(k.is_string())
            pimpl->stringa = k.get_string();

        if(k.is_list())
            pimpl->testa_lista= nullptr;

        if(k.is_list())
            pimpl->coda_lista= nullptr;

        if(k.is_dictionary())
            pimpl->testa_diz= nullptr;

        if(k.is_dictionary())
            pimpl->coda_diz= nullptr;

        pimpl->t=k.pimpl->t;
    }
     */

    //remove constructor
    //r punta al json e
    //una & è una left value reference ed è come dare un secondo nome alla variabile
    //una && è una right value reference è un valore che potenzialmete scomare dopo l'utilizzo
    //pimpl puntava a nullptr e r.pimpl puntava al valore
    //ho fatto si che pimpl puntasse al valore nache lui
    //poi ho distrutto r
    json::json(json&& r) {
        pimpl=r.pimpl;
        r.pimpl= nullptr;
    }








void json::impl::svuota_lista(){
    while(testa_lista!= nullptr){
        lista* temp=testa_lista;
        testa_lista=testa_lista->next;
        delete temp;
    }
}

void json::impl::svuota_dizionario(){
    while(testa_diz!= nullptr){
        dizionario* temp=testa_diz;
        testa_diz=testa_diz->next;
        delete temp;
    }
}










//funzioni bool
bool json::is_bool() const {
    if(pimpl->t==bol){
        return true;
    }
    return false;
}

bool json::is_dictionary() const {
    if(pimpl->t==diz){
        return true;
    }
    return false;
}

bool json::is_string() const {
    if(pimpl->t==str){
        return true;
    }
    return false;
}

bool json::is_number() const {
    if(pimpl->t==num){
        return true;
    }
    return false;
}

bool json::is_list() const {
    if(pimpl->t==lis){
        return true;
    }
    return false;
}

bool json::is_null() const {
    if(pimpl->t==nullo){
        return true;
    }
    return false;
}











//funzioni get
double& json::get_number() {
    if(is_number())
        return pimpl->numero;
    throw json_exception{"E' STATA LANCIATA LA FUNZIONE GET_NUMBER MA NON E' UN NUMERO!"};
}

double const& json::get_number() const{
    if(is_number())
        return pimpl->numero;
    throw json_exception{"E' STATA LANCIATA LA FUNZIONE GET_NUMBER MA NON E' UN NUMERO!"};
}

bool& json::get_bool() {
    if(is_bool())
        return pimpl->booleano;
    throw json_exception{"E' STATA LANCIATA LA FUNZIONE GET_BOOL MA NON E' UN BOOLEANO!"};
}

bool const& json::get_bool() const{
    if(is_bool())
        return pimpl->booleano;
    throw json_exception{"E' STATA LANCIATA LA FUNZIONE GET_BOOL MA NON E' UN BOOLEANO!"};
}

std::string& json::get_string() {
    if(is_string())
        return pimpl->stringa;
    throw json_exception{"E' STATA LANCIATA LA FUNZIONE GET_STRING MA NON E' UNA STRINGA!"};
}

std::string const& json::get_string() const{
    if(is_string())
        return pimpl->stringa;
    throw json_exception{"E' STATA LANCIATA LA FUNZIONE GET_STRING MA NON E' UNA STRINGA!"};
}










//funzioni set
//prendere la stinga , metterla all'interno del json e dirgli tu sei una stringa
void json::set_string(std::string const& s){
    if(pimpl->t==lis){
        pimpl->svuota_lista();
    }
    if(pimpl->t==diz){
        pimpl->svuota_dizionario();
    }
    pimpl->t=str;  //setto il tipo a "stringa"
    pimpl->stringa = s;  //inserisco la stringa nello json
}

//set bool e number sono uguali a stringa
void json::set_bool(bool b){
    if(pimpl->t==lis){
        pimpl->svuota_lista();
    }
    if(pimpl->t==diz){
        pimpl->svuota_dizionario();
    }
    pimpl->t=bol;  //setto il tipo a "stringa"
    pimpl->booleano = b;  //inserisco la stringa nello json
}

void json::set_number(double d) {
    if(pimpl->t==lis){
        pimpl->svuota_lista();
    }
    if(pimpl->t==diz){
        pimpl->svuota_dizionario();
    }
    pimpl->t=num;  //setto il tipo a "stringa"
    pimpl->numero = d;  //inserisco la stringa nello json
}

void json::set_null() {
    if(pimpl->t==lis){
        pimpl->svuota_lista();
    }
    if(pimpl->t==diz){
        pimpl->svuota_dizionario();
    }
    pimpl->t=nullo;
}

void json::set_list() {
    if(pimpl->t==lis){
        pimpl->svuota_lista();
    }
    if(pimpl->t==diz){
        pimpl->svuota_dizionario();
    }
    pimpl->t=lis;  //setto il tipo a "stringa"
}

void json::set_dictionary() {
    if(pimpl->t==lis){
        pimpl->svuota_lista();
    }
    if(pimpl->t==diz){
        pimpl->svuota_dizionario();
    }
    pimpl->t=diz;  //setto il tipo a "stringa"
}





//push è aggiungo e pop è tolgo
void json::push_back(const json & j) {
    if(pimpl->t!= lis){
        throw json_exception{"HAI CHIAMATO PUSH_BACK IN UN JSON CHE NON E' UNA LISTA!!"};
    }
    lista* nodo=new lista{j, nullptr};
    if(pimpl->testa_lista== nullptr){
        pimpl->testa_lista=nodo;
        pimpl->coda_lista=nodo;
    }else {
        pimpl->coda_lista->next = nodo;
        pimpl->coda_lista = nodo;
    }
}//DA AGGIUNGERE PUSH FRONT IN TESTA  void push_front(json const&);

void json::insert(std::pair<std::string, json> const& c) {
    if(pimpl->t!= diz){
        throw json_exception{"HAI CHIAMATO INSERT IN UN JSON CHE NON E' UN DIZIONARIO!!"};
    }
    dizionario* nodo=new dizionario{c, nullptr};
    if(pimpl->testa_diz== nullptr){
        pimpl->testa_diz=nodo;
        pimpl->coda_diz=nodo;
    }else {
        pimpl->coda_diz->next = nodo;
        pimpl->coda_diz = nodo;
    }
}









    //OPERATOR=
    json& json::operator=(json&& k){
        if(this==&k){
            return *this;
        }

        //this è la classe su cui sono cioè quello a sinistra
        if(pimpl){
            pimpl->svuota_dizionario();
            pimpl->svuota_lista();
            delete pimpl;
        }

        pimpl=k.pimpl;
        k.pimpl= nullptr;

        return *this;
    }





/*
//OVERLOADING DEGLI OPERATORI[]
    json const& json::operator[](std::string const&) const {

    }
    */

//funzioni globali quindi non posso accedere alle variabili private
//ma uso le funzioni create
//cin>>x prende il valore di cin(che scrivi sulla tastiera e lo salva dentro x)
//lhs è il nostro stram di input
//rhs è dove vuoi salvare
//usiamo una variabile di stream generale perchè dobbiamo riuscire a prendere qualsiasi tipo di input
//che sia file , cin ecc in attesa che siano letti
//lhs contiene il file json con tutti i suoi caratteri e la loro organizzazione
//dobbiamo riuscire a salvare il suo contenuto all'interno di rhs
//ogni tipo inizia con un carattere diverso
//char per bool : t   o   f
//char per string : " "
//char per nullo : n
//char per double : da 0 a 9
//char per lista : [ ]
//char per dizionario : { }









/*DA FARE
 * ITERATORI
 * LE LORO FUNZIONI
 * SCRITTURA E LETTURA DA FILE
 * OVERLOADING DEGLI OPERATORI
 * */









//iteratori: insieme tra il puntatore e le sue funzioni
// *******LIST****
struct json::list_iterator
{
public:
    //sono tutte funzioni che ti permettono di muovere il puntatore
    list_iterator(lista* p); //costruttore che deve prendere il puntatore ptr che punta p
    json& operator*() const;
    json* operator->() const;
    list_iterator& operator++();
    list_iterator operator++(int);
    bool operator==(list_iterator const&) const;
    bool operator!=(list_iterator const&) const;
    lista* ptr; //è un puntatore a lista
};


//voglio accedere alla funzione list_iterato che si trova all'interno alla funzione
//list_iterator vhe si trova all'interno del json
json::list_iterator::list_iterator(lista* p){
    ptr=p;
}

json& json::list_iterator::operator*() const{
    return ptr->info; //ritorno il dato a cui punta ptr
}

json* json::list_iterator::operator->() const{
    return &(ptr->info); //indirizzo del dato
}

//operatore di pre incremento
//per reference perchè vado a prelevare un dato che ho  già
json::list_iterator& json::list_iterator::operator++(){
    ptr=ptr->next;
    return *this;  //con this ritorno l'iteratore stesso (this è un puntatore quindi
    //devo deferenziarlo
}

//operatore di post incremento
//senza reference perchè vado a prelevare successivamente un dato che ho creato e copiato
json::list_iterator json::list_iterator::operator++(int){
    list_iterator i = ptr;
    ptr=ptr->next;
    return i;  //i un iteratore
}

//guardare de ptr della classe punta allo stesso ptr di i
bool json::list_iterator::operator==(list_iterator const& i) const{
    return ptr==i.ptr;  //la freccia la usi se quello da cui parti è un puntatore
}

bool json::list_iterator::operator!=(list_iterator const& i) const{
    return ptr!=i.ptr;  //la freccia la usi se quello da cui parti è un puntatore
}







//******************************
//******************************
//AGGIUNGERE I CONST

//iteratori: insieme tra il puntatore e le sue funzioni
// *******LIST****
struct json::const_list_iterator
{
public:
    //sono tutte funzioni che ti permettono di muovere il puntatore
    const_list_iterator(lista* p); //costruttore che deve prendere il puntatore ptr che punta p
    json& operator*() const;
    json* operator->() const;
    const_list_iterator& operator++();
    const_list_iterator operator++(int);
    bool operator==(const_list_iterator const&) const;
    bool operator!=(const_list_iterator const&) const;
    lista* ptr; //è un puntatore a lista
};


//voglio accedere alla funzione list_iterato che si trova all'interno alla funzione
//list_iterator vhe si trova all'interno del json
json::const_list_iterator::const_list_iterator(lista* p){
    ptr=p;
}

json& json::const_list_iterator::operator*() const{
    return ptr->info; //ritorno il dato a cui punta ptr
}

json* json::const_list_iterator::operator->() const{
    return &(ptr->info); //indirizzo del dato
}

//operatore di pre incremento
json::const_list_iterator& json::const_list_iterator::operator++(){
    ptr=ptr->next;
    return *this;  //con this ritorno l'iteratore stesso (this è un puntatore quindi
    //devo deferenziarlo
}

//operatore di post incremento
json::const_list_iterator json::const_list_iterator::operator++(int){
    const_list_iterator i = ptr;
    ptr=ptr->next;
    return i;  //i un iteratore
}

//guardare de ptr della classe punta allo stesso ptr di i
bool json::const_list_iterator::operator==(const_list_iterator const& i) const{
    return ptr==i.ptr;  //la freccia la usi se quello da cui parti è un puntatore
}

bool json::const_list_iterator::operator!=(const_list_iterator const& i) const{
    return ptr!=i.ptr;  //la freccia la usi se quello da cui parti è un puntatore
}





// ****DIZIONARI***
struct json::dictionary_iterator
{
public:
    dictionary_iterator(dizionario*);
    std::pair<std::string,json>& operator*() const;
    std::pair<std::string,json>* operator->() const;
    dictionary_iterator& operator++();
    dictionary_iterator operator++(int);
    bool operator==(dictionary_iterator const&) const;
    bool operator!=(dictionary_iterator const&) const;
    dizionario* ptr;
};

//voglio accedere alla funzione list_iterato che si trova all'interno alla funzione
//list_iterator vhe si trova all'interno del json
json::dictionary_iterator::dictionary_iterator(dizionario* p){
    ptr=p;
}

std::pair<std::string,json>& json::dictionary_iterator::operator*() const{
    return ptr->info; //ritorno il dato a cui punta ptr
}

std::pair<std::string,json>* json::dictionary_iterator::operator->() const{
    return &(ptr->info); //indirizzo del dato
}

//operatore di pre incremento
json::dictionary_iterator& json::dictionary_iterator::operator++(){
    ptr=ptr->next;
    return *this;  //con this ritorno l'iteratore stesso (this è un puntatore quindi
    //devo deferenziarlo
}

//operatore di post incremento
json::dictionary_iterator json::dictionary_iterator::operator++(int){
    dictionary_iterator i = ptr;
    ptr=ptr->next;
    return i;  //i un iteratore
}

//guardare de ptr della classe punta allo stesso ptr di i
bool json::dictionary_iterator::operator==(dictionary_iterator const& i) const{
    return ptr==i.ptr;  //la freccia la usi se quello da cui parti è un puntatore
}

bool json::dictionary_iterator::operator!=(dictionary_iterator const& i) const{
    return ptr!=i.ptr;  //la freccia la usi se quello da cui parti è un puntatore
}





//**CONST DICTIONARY
//AGGIUNGERE I CONST  , fatto
struct json::const_dictionary_iterator
{
public:
    const_dictionary_iterator(dizionario*);
    std::pair<std::string,json>& operator*() const;
    std::pair<std::string,json>* operator->() const;
    const_dictionary_iterator& operator++();
    const_dictionary_iterator operator++(int);
    bool operator==(const_dictionary_iterator const&) const;
    bool operator!=(const_dictionary_iterator const&) const;
    dizionario* ptr;
};

//voglio accedere alla funzione list_iterato che si trova all'interno alla funzione
//list_iterator vhe si trova all'interno del json
json::const_dictionary_iterator::const_dictionary_iterator(dizionario* p){
    ptr=p;
}

std::pair<std::string,json>& json::const_dictionary_iterator::operator*() const{
    return ptr->info; //ritorno il dato a cui punta ptr
}

std::pair<std::string,json>* json::const_dictionary_iterator::operator->() const{
    return &(ptr->info); //indirizzo del dato
}

//operatore di pre incremento
json::const_dictionary_iterator& json::const_dictionary_iterator::operator++(){
    ptr=ptr->next;
    return *this;  //con this ritorno l'iteratore stesso (this è un puntatore quindi
    //devo deferenziarlo
}

//operatore di post incremento
json::const_dictionary_iterator json::const_dictionary_iterator::operator++(int){
    const_dictionary_iterator i = ptr;
    ptr=ptr->next;
    return i;  //i un iteratore
}

//guardare de ptr della classe punta allo stesso ptr di i
bool json::const_dictionary_iterator::operator==(const_dictionary_iterator const& i) const{
    return ptr==i.ptr;  //la freccia la usi se quello da cui parti è un puntatore
}

bool json::const_dictionary_iterator::operator!=(const_dictionary_iterator const& i) const{
    return ptr!=i.ptr;  //la freccia la usi se quello da cui parti è un puntatore
}










//FUNZIONI
//devo ritornare l'iteratore che punta al primo elemento della lista
json::list_iterator json::begin_list(){
    if(is_list()){
        list_iterator i = pimpl->testa_lista;  //voglio che l'iteretore punti alla testa della lista
        return i;
       }else{
        throw json_exception{"HAI PROVATO A RITORNARE UN ITERATORE CHE NON PUNTA UNA LISTA!!!, BEGIN_LIST()"};
    }
}
json::const_list_iterator json::begin_list() const{
    if(is_list()){
        const_list_iterator i = pimpl->testa_lista;  //voglio che l'iteretore punti alla testa della lista
        return i;
    }else{
        throw json_exception{"HAI PROVATO A RITORNARE UN ITERATORE CHE NON PUNTA UNA LISTA!!!, BEGIN_LIST()"};
    }
}

//devo ritornare l'iteratore che punta al successivo dell'ultimo elemento della lista cioè nullptr
json::list_iterator json::end_list(){
    if(is_list()){
        list_iterator i = nullptr;  //voglio che l'iteretore punti alla testa della lista
        return i;
    }else{
        throw json_exception{"HAI PROVATO A RITORNARE UN ITERATORE CHE NON PUNTA UNA LISTA!!!, END_LIST()"};
    }
}

json::const_list_iterator json::end_list() const{
    if(is_list()){
        const_list_iterator i = nullptr;  //voglio che l'iteretore punti alla testa della lista
        return i;
    }else{
        throw json_exception{"HAI PROVATO A RITORNARE UN ITERATORE CHE NON PUNTA UNA LISTA!!!, END_LIST()"};
    }
}









json::dictionary_iterator json::begin_dictionary(){
    if(is_dictionary()){
        dictionary_iterator i = pimpl->testa_diz;  //voglio che l'iteretore punti alla testa della lista
        return i;
    }else {
        throw json_exception{"HAI PROVATO A RITORNARE UN ITERATORE CHE NON PUNTA AD UN DIZIONARIO!!!, BEGIN_DICTIONARY()"};
    }
}

json::const_dictionary_iterator json::begin_dictionary() const{
    if(is_dictionary()){
        const_dictionary_iterator i = pimpl->testa_diz;  //voglio che l'iteretore punti alla testa della lista
        return i;
    }else{
        throw json_exception{"HAI PROVATO A RITORNARE UN ITERATORE CHE NON PUNTA AD UN DIZIONARIO!!!, BEGIN_DICTIONARY()"};
    }
}




json::dictionary_iterator json::end_dictionary(){
    if(is_dictionary()){
        dictionary_iterator i = nullptr;  //voglio che l'iteretore punti alla testa della lista
        return i;
    }else{
        throw json_exception{"HAI PROVATO A RITORNARE UN ITERATORE CHE NON PUNTA AD UN DIZIONARIO!!!, END_DICTIONARY()"};
    }
}

json::const_dictionary_iterator json::end_dictionary() const{
    if(is_dictionary()){
        const_dictionary_iterator i = nullptr;  //voglio che l'iteretore punti alla testa della lista
        return i;
    }else{
        throw json_exception{"HAI PROVATO A RITORNARE UN ITERATORE CHE NON PUNTA AD UN DIZIONARIO!!!, END_DICTIONARY()"};
    }
}

//operato=
json& json::operator=(json const& k){

    //devo contralle che k non sia this , cioè un auto assegnamento 9=9
    //confronto gli indirizzi delle due classi per vedere che non puntino nello stesso
    if(this==&k){
        return *this;
    }

    //this è la classe su cui sono cioè quello a sinistra
    if(pimpl){
        pimpl->svuota_dizionario();
        pimpl->svuota_lista();
    }
    if(k.is_number())
        pimpl->numero = k.get_number();

    if(k.is_bool())
        pimpl->booleano=false;

    if(k.is_string())
        pimpl->stringa = k.get_string();

    if(k.is_list())
        pimpl->testa_lista= nullptr;

    if(k.is_list()) {
        pimpl->coda_lista = nullptr;
        for (json::const_list_iterator it = k.begin_list(); it != k.end_list(); ++it){
            pimpl->list_append(it.ptr->info);
        }
    }

    if(k.is_dictionary()) {
        pimpl->testa_diz = nullptr;
        for (json::const_dictionary_iterator it = k.begin_dictionary(); it != k.end_dictionary(); ++it){
            pimpl->dict_append(it.ptr->info);
        }
    }

    if(k.is_dictionary())
        pimpl->coda_diz= nullptr;

    pimpl->t=k.pimpl->t;

    return *this;
    //this è un puntatore alla classe quindi devo sempre deferenziarlo
    //devo ritornare per la concatenazione del operatore =
}












//dentro le [] ci metto la stringa(chiave) e mi ritorna il json
//corrispondente a quella chiave
json const& json::operator[](std::string const& s) const{
    if(!is_dictionary()){
        throw json_exception{"NON E' UN DIZIONARIO , OPERATOR[]!!"};
    }
    //SE TROVO LA STRINGA s NEL DIZIONARIO RITORNO IL SUO JSON ALTRIMENTI ECCEZZIONE
    dizionario *d= pimpl->testa_diz;
    while(d!= nullptr){
        if(d->info.first==s){
            return d->info.second; //ritorno il secondo del pair cioè il json
        }
    }
    throw json_exception{"CHIAVE NON TROVATA, OPERATOR[]!!"};
}


//json& json::operator[](std::string const&);
json& json::operator[](std::string const& s) {
    if(!is_dictionary()){
        throw json_exception{"NON E' UN DIZIONARIO , OPERATOR[]!!"};
    }
    //SE TROVO LA STRINGA s NEL DIZIONARIO RITORNO IL SUO JSON ALTRIMENTI ECCEZZIONE
    dizionario *d= pimpl->testa_diz;
    while(d!= nullptr){
        if(d->info.first==s){
            return d->info.second; //ritorno il secondo del pair cioè il json
        }
    }
    //creo un nuovo nodo e lo butto dentro con insert
    json nuovo;
    std::pair<std::string,json> nuovo2 (s,nuovo);
    insert(nuovo2);
    return pimpl->coda_diz->info.second; //ritorniamo il json
}



/*
struct dictionary_iterator;
struct const_list_iterator;
struct const_dictionary_iterator;
 */





//funzione per leggere il booleano
void leggi_booleano(std::istream& lhs, json& rhs){
    //mi estraggo i carattere e li metto in una stringa e poi con un confronto
    // capire se è TRUE FALSE o NESSUN ALTRO
    char carattere;
    string s="";
    for(int i=0;i<5;i++){
        lhs>>carattere;
        s+=carattere;
    }

    if(s=="false"){
        rhs.set_bool(false);
    }else{
        s.pop_back();
        lhs.putback(carattere);//rimette dentro lhs il carattre tolto che mi sarebbe servito
        //per capire il tipo di dato sucessivo al bool
        if(s=="true"){
            std::cout<<"è true"<<std::endl;
            rhs.set_bool(true);
        }else{
            throw json_exception{"NEL METOTO LEGGI_BOOL IL BOOL NON E' UN BOOLEANO!!"};
        }
    }

}
//funzione per leggere la stringa
void leggi_stringa(std::istream& lhs, json& rhs){
    bool verifica= true;
    char carattere;
    string s="";

    while(verifica){
        lhs.get(carattere); //questa funzione mi estrae anche gli spazi
        if(carattere!= '\\' && carattere != '"'){
            s+=carattere;
        }else{
            if(carattere=='"'){
                rhs.set_string(s);
                verifica = false;
            }else{
                s+=carattere;
                lhs.get(carattere);
                s+=carattere;
            }
        }
    }

}

void leggi_nullo(std::istream& lhs, json& rhs){
    char carattere;
    string s="";
    for(int i=0;i<4;i++){
        lhs>>carattere;
        s+=carattere;
    }
    if(s=="null"){
        rhs.set_null();
    }else{
        throw json_exception{"NEL METODO LEGGI_NULL NON E' STATO LETTO UN NULL!!"};
    }
}

void leggi_double(std::istream& lhs, json& rhs){
    double numero;
    lhs>>numero;   //con il double l'operatore cin>> estrae completamente il numero
    rhs.set_number(numero);
}

void leggi_lista(std::istream& lhs, json& rhs){

    json j;
    lhs>>j; //richiamo la funzione operator>> creata , ricorsione indiretta
    rhs.push_back(j);  //aggiungo il nodo alla lista
    char carattere;
    lhs>>carattere;
    if(carattere==','){
        leggi_lista(lhs,rhs);
    }else if(carattere!=']'){
        throw json_exception{"SEI POCO SVEGLIO, SVEGLIATI, NON E' UN NODO DELLA LISTA, leggi_lista!! "};
    }
}


void leggi_dizionario(std::istream& lhs, json& rhs){
    string s="";
    char c2;
    lhs>>c2;
    if(c2=='"'){

        bool verifica= true;
        char carattere;

        while(verifica){
            lhs.get(carattere); //questa funzione mi estrae anche gli spazi
            if(carattere!= '\\' && carattere != '"'){
                s+=carattere;
            }else{
                if(carattere=='"'){
                    verifica = false;
                }else{
                    s+=carattere;
                    lhs.get(carattere);
                    s+=carattere;
                }
            }
        }
        //cout<<"s = "<<s<<endl;
    }else{
        throw json_exception{"NON E' UN DIZIONARIO, MANCA LA CHIAVE, leggi_dizionario"};
    }

    lhs>>c2;
    if(c2!=':'){
        throw json_exception{"NON E' UN DIZIONARIO, MANCANO I :, leggi_dizionario"};
    }
    json j;

    lhs>>j; //richiamo la funzione operator>> creata , ricorsione indiretta
    
    std::pair<std::string, json> nome {s,j};
    
    rhs.insert(nome);

    char carattere;
    lhs>>carattere;
    
    if(carattere==','){
        leggi_dizionario(lhs,rhs);
    }else if(carattere!='}'){
        std::cout<<"stampa prova";
        throw json_exception{"SEI POCO SVEGLIO, SVEGLIATI, NON E' UN NODO DELLA LISTA, leggi_lista!!"};
    }
}







std::istream& operator>>(std::istream& lhs, json& rhs) {
    //mi creo un variabile per tirarmi fuori i caratteri uno alla volta
    char carattere;
    lhs >> carattere;  //è come fare cin>>carattere
    if (lhs.eof()) {   //se è alla fine del file non devo fare nulla con il carattere
        return lhs;
    }
    if (carattere == 't' || carattere == 'f') {
        lhs.putback(carattere);
        leggi_booleano(lhs, rhs);
    } else if (carattere == '"') {
        leggi_stringa(lhs, rhs);
    } else if (carattere == 'n') {
        lhs.putback(carattere);
        leggi_nullo(lhs, rhs);
    } else if (carattere >= '0' && carattere <= '9') {
        lhs.putback(carattere);
        leggi_double(lhs, rhs);
    } else if (carattere == '[') {
        rhs.set_list();
        lhs >> carattere;
        if (carattere != ']') {
            lhs.putback(carattere); //se il carattere sucessivo che ci siamo mangiati
            //non è ']' allora lo riportiamo "putback()"
            leggi_lista(lhs, rhs);
        }
    } else if (carattere == '{') {
        rhs.set_dictionary();
        lhs >> carattere;
        if (carattere != '}') {
            lhs.putback(carattere);
            leggi_dizionario(lhs, rhs);
        }
    } else {
        throw json_exception{"IL CONTENUTO DEL JSON NON RISPETTA ALCUN TIPO NE REGOLA , OPERATOR"};
    }
    return lhs; //ritorno lhs per la questione concatenaz
}






void scrivi_booleano(std::ostream& lhs, json const& rhs){
    //lhs è il nostro output stream e c'è la buttiamo dentro la roba
    if(rhs.get_bool()){
        lhs<<"true";
    }else{
        lhs<<"false";
    }
}

void scrivi_stringa(std::ostream& lhs, json const& rhs){
    lhs<<'"'<<rhs.get_string()<<'"';
}

void scrivi_nullo(std::ostream& lhs){
    lhs<<"null";
}

void scrivi_double(std::ostream& lhs, json const& rhs){
    lhs<<rhs.get_number();
}

void scrivi_lista(std::ostream& lhs, json const& rhs){
    lhs<<'[';
    //ciclo for each solo quando ho dei iteratori definiti
    //std::list, std::vector si usano i cicli for each
    //for(auto element : lista)
    //auto da il tipo giusto in modo automatico
    bool first=true;
    for(auto it=rhs.begin_list();it!=rhs.end_list();++it){
        //quel *it è quello che nel for each chiamiamo element
        if(!first) {
            lhs << ',';
        }
        first=false;
        lhs<<*it;  //e come dire it->info

    }
    lhs<<']';
}

void scrivi_dizionario(std::ostream& lhs, json const& rhs){
    lhs<<'{';
    //ciclo for each solo quando ho dei iteratori definiti
    //std::list, std::vector si usano i cicli for each
    //for(auto element : lista)
    //auto da il tipo giusto in modo automatico
    bool first=true;
    for(auto it=rhs.begin_dictionary();it!=rhs.end_dictionary();++it){
        //quel *it è quello che nel for each chiamiamo element
        if(!first) {
            lhs << ',';
        }
        first=false;
        lhs<<'"'<<it->first<<'"';  //it-> è come scrivere info->first
        lhs<<':';
        lhs<<it->second;
    }
    lhs<<'}';
}

//lhs
//verifico il json di che tipo è , e lo scrivo
std::ostream& operator<<(std::ostream& lhs, json const& rhs){
    if(rhs.is_bool()){
        scrivi_booleano(lhs, rhs);
    }else if(rhs.is_string()){
        scrivi_stringa(lhs, rhs);
    }else if(rhs.is_null()){
        scrivi_nullo(lhs);
    }else if(rhs.is_number()){
        scrivi_double(lhs, rhs);
    }else if(rhs.is_list()){
        scrivi_lista(lhs, rhs);
    }else if(rhs.is_dictionary()){
        scrivi_dizionario(lhs, rhs);
    }else{
        throw json_exception{"IL CONTENUTO DEL JSON NON RISPETTA ALCUN TIPO NE REGOLA , OPERATOR>>"};
    }
    return lhs; //ritorno lhs per la questione concatenazione
}


int main() {
    /*
    json j;
    std::cin>>j;
    std::cout<<j<<std::endl;
*/

    std::string newFile = "";
    for (int i=1; i<102;++i){
        newFile = "C:\\Users\\ilmio\\Desktop\\Progetto_Pel_2023\\json_test\\"+std::to_string(i);
        newFile += ".txt";
        std::cout<<""<<std::endl;
        std::cout<<""<<std::endl;
        std::cout<<"Apro il file "<<i<<std::endl;
        std::ifstream file(newFile);
        if (!file.is_open()) {
            std::cout << "Failed to open the file." << std::endl;
            return 1;
        }
        std::stringstream ss;
        ss << file.rdbuf();
        file.close();

        json data;

        ss >> data;
        std::cout << data << std::endl;
    }



/*
    std::ifstream file("C:\\Users\\ilmio\\Desktop\\Progetto_PeL_2023\\prova_json.txt");
    if (!file.is_open()) {
        //std::cout << "Failed to open the file." << std::endl;
        return 1;
    }
    std::stringstream ss;
    ss << file.rdbuf();
    file.close();

    json data;

    ss >> data;
    std::cout << data << std::endl;

    //std::cout<<"Il parsing e' andato a buon fine"<<std::endl;

    return 0;
    */
}