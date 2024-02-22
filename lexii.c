#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define TAILLEMEM 200
#define TAILLECODE 200

typedef enum
{
    ID_TOKEN,
    PROGRAM_TOKEN,
    CONST_TOKEN,
    VAR_TOKEN,
    BEGIN_TOKEN,
    END_TOKEN,
    IF_TOKEN,
    THEN_TOKEN,
    WHILE_TOKEN,
    DO_TOKEN,
    READ_TOKEN,
    WRITE_TOKEN,
    PV_TOKEN,
    PT_TOKEN,
    PLUS_TOKEN,
    MOINS_TOKEN,
    MULT_TOKEN,
    DIV_TOKEN,
    VIR_TOKEN,
    AFF_TOKEN,
    INF_TOKEN,
    INFEG_TOKEN,
    SUP_TOKEN,
    SUPEG_TOKEN,
    DIFF_TOKEN,
    PO_TOKEN,
    PF_TOKEN,
    FIN_TOKEN,
    NUM_TOKEN,
    ERREUR_TOKEN,
    EOF_TOKEN,
    EG_TOKEN,REPEAT_TOKEN,UNTIL_TOKEN,FOR_TOKEN,ELSE_TOKEN,CASE_TOKEN,OF_TOKEN,
    INTO_TOKEN,DOWNTO_TOKEN,
    DDOT_TOKEN
} CODES_LEX;



// erreur types
typedef enum
{
    ID_ERR,
    PROGRAM_ERR,
    CONST_ERR,
    VAR_ERR,
    BEGIN_ERR,
    END_ERR,
    IF_ERR,
    THEN_ERR,
    WHILE_ERR,
    DO_ERR,
    READ_ERR,
    WRITE_ERR,
    PV_ERR,
    PT_ERR,
    PLUS_ERR,
    MOINS_ERR,
    MULT_ERR,
    DIV_ERR,
    VIR_ERR,
    AFF_ERR,
    INF_ERR,
    INFEG_ERR,
    SUP_ERR,
    SUPEG_ERR,
    DIFF_ERR,
    PO_ERR,
    PF_ERR,
    FIN_ERR,
    NUM_ERR,
    ERREUR_ERR,
    EOF_ERR,
    EG_ERR,
    CONST_VAR_BEGIN_ERR,
    VAR_BEGIN_ERR,REPEAT_ERR,UNTIL_ERR,FOR_ERR,ELSE_ERR,CASE_ERR,OF_ERR,
    INTO_ERR,DOWNTO_ERR,DDOT_ERR, INST_PCODE_ERR
} CODES_ERR;

typedef struct
{
    CODES_LEX CODE;
    char NOM[20];
    int val;
} TSym_Cour;

TSym_Cour SYM_COUR;

FILE *fichier;
FILE *FICH_SORTIE;

int nbreIdfs = 0;
typedef enum { TPROG, TCONST, TVAR } TSYM;

typedef struct {
    char NOM[20];
    TSYM TIDF;
    int val;
    int adresse;
} T_TAB_IDF;

T_TAB_IDF TAB_IDFS[100];

//pile de la machine
int MEM[TAILLEMEM];
//pointeur de pile
int SP;
int offset = -1;

typedef enum {
    ADD,SUB,MUL,DIV,EQL,NEQ,GTR,
    LSS,GEQ,LEQ, PRN,INN,INT,LDI,LDA,LDV,
    STO,BRN,BZE,HLT
} MNEMONIQUES;

typedef struct {
    MNEMONIQUES MNE;
    int suite;
}INSTRUCTION;

INSTRUCTION PCODE[TAILLECODE];


int pc = 0;




char Car_Cour; // caractère courant
const char * afficherToken(CODES_LEX code) {
    switch (code) {
        case EG_TOKEN: return "EG_TOKEN";
        case ID_TOKEN: return "ID_TOKEN";
        case PROGRAM_TOKEN: return "PROGRAM_TOKEN";
        case CONST_TOKEN: return "CONST_TOKEN";
        case VAR_TOKEN: return "VAR_TOKEN";
        case BEGIN_TOKEN: return "BEGIN_TOKEN";
        case END_TOKEN: return "END_TOKEN";
        case IF_TOKEN: return "IF_TOKEN";
        case THEN_TOKEN: return "THEN_TOKEN";
        case WHILE_TOKEN: return "WHILE_TOKEN";
        case DO_TOKEN: return "DO_TOKEN";
        case READ_TOKEN: return "READ_TOKEN";
        case WRITE_TOKEN: return "WRITE_TOKEN";
        case PV_TOKEN: return "PV_TOKEN";
        case PT_TOKEN: return "PT_TOKEN";
        case PLUS_TOKEN: return "PLUS_TOKEN";
        case MOINS_TOKEN: return "MOINS_TOKEN";
        case MULT_TOKEN: return "MULT_TOKEN";
        case DIV_TOKEN: return "DIV_TOKEN";
        case VIR_TOKEN: return "VIR_TOKEN";
        case AFF_TOKEN: return "AFF_TOKEN";
        case INF_TOKEN: return "INF_TOKEN";
        case INFEG_TOKEN: return "INFEG_TOKEN";
        case SUP_TOKEN: return "SUP_TOKEN";
        case SUPEG_TOKEN: return "SUPEG_TOKEN";
        case DIFF_TOKEN: return "DIFF_TOKEN";
        case PO_TOKEN: return "PO_TOKEN";
        case PF_TOKEN: return "PF_TOKEN";
        case FIN_TOKEN: return "FIN_TOKEN";
        case NUM_TOKEN: return "NUM_TOKEN";
        case ERREUR_TOKEN: return "ERREUR_TOKEN";
        case EOF_TOKEN: return "EOF_TOKEN";
        default: return "UNKNOWN_TOKEN";
    }
}

void VARS();
void INSTS();
void INST();
void AFFEC();
void SI();
void TANTQUE();
void ECRIRE();
void LIRE();
void EXPR();
void TERM();
void FACT();
void MULOP();
void ADDOP();
void RELOP();
void COND();
void Lire_Car();
void Erreur(CODES_ERR code);
void Test_Symbole(CODES_LEX cl, CODES_ERR COD_ERR);
void PROGRAM();
void BLOCK();
void CONSTS();
void Sym_Suiv();
void lire_mot();
void lire_nombre();
void CAS();
void POUR();
void REPETER();
void ajouter_idf(char* nom, TSYM type);
int idf_existe(char* nom);
void generer1(MNEMONIQUES mne);
void generer2(MNEMONIQUES mne, int suite);
void SaveInstToFile(INSTRUCTION INST);
void SavePCodeToFile();
void INTER_INST(INSTRUCTION INST);
void INTER_PCODE();

void SaveInstToFile(INSTRUCTION INST)
{
 switch( INST.MNE){
 case LDA: fprintf(FICH_SORTIE, "%s \t %d \n", "LDA", INST.suite); break;
 case LDI: fprintf(FICH_SORTIE, "%s \t %d \n", "LDI", INST.suite); break;
 case INT: fprintf(FICH_SORTIE, "%s \t %d \n", "INT", INST.suite); break;
 case BZE: fprintf(FICH_SORTIE, "%s \t %d \n", "BZE", INST.suite); break;
 case BRN: fprintf(FICH_SORTIE, "%s \t %d \n", "BRN", INST.suite); break;
 case LDV: fprintf(FICH_SORTIE, "%s \n", "LDV"); break;
 case ADD: fprintf(FICH_SORTIE, "%s \n", "ADD"); break;
 case SUB: fprintf(FICH_SORTIE, "%s \n", "SUB"); break;
 case MUL: fprintf(FICH_SORTIE, "%s \n", "MUL"); break;
 case DIV: fprintf(FICH_SORTIE, "%s \n", "DIV"); break;
 case LEQ: fprintf(FICH_SORTIE, "%s \n", "LEQ"); break;
 case GEQ: fprintf(FICH_SORTIE, "%s \n", "GEQ"); break;
 case NEQ: fprintf(FICH_SORTIE, "%s \n", "NEQ"); break;
 case EQL: fprintf(FICH_SORTIE, "%s \n", "EQL"); break;
 case LSS: fprintf(FICH_SORTIE, "%s \n", "LSS"); break;
 case GTR: fprintf(FICH_SORTIE, "%s \n", "GTR"); break;
 case HLT: fprintf(FICH_SORTIE, "%s \n", "HLT"); break;
 case STO: fprintf(FICH_SORTIE, "%s \n", "STO"); break;
 case INN: fprintf(FICH_SORTIE, "%s \n", "INN"); break;
 case PRN: fprintf(FICH_SORTIE, "%s \n", "PRN"); break;
 default: Erreur(INST_PCODE_ERR); break;
 }
 }

 void SavePCodeToFile(){
    for (int i=0; i<=pc; i++){
        SaveInstToFile(PCODE[i]);
    }
 }

 void INTER_INST(INSTRUCTION INST){
    int val1, adr, val2;
    switch(INST.MNE){
        case INT: 
            offset=SP=INST.suite;
            pc++;
            break;
        case LDI: 
            MEM[++SP]=INST.suite; 
            pc++;
            break;
        case LDA:
            MEM[++SP]=INST.suite;
            pc++;
            break;
        case STO:
             val1=MEM[SP--]; 
             adr=MEM[SP--];
             MEM[adr]=val1;
             pc++;
            break;
        case LDV: 
            adr=MEM[SP--]; 
            MEM[++SP]=MEM[adr];
            pc++;
            break;
        case EQL:
            val1=MEM[SP--];
            val2=MEM[SP--];
            MEM[++SP]=(val1==val2); 
            pc++;
            break;
        case LEQ:
            val2=MEM[SP--];
            val1=MEM[SP--];
            MEM[++SP]=(val1<=val2); 
            pc++;
            break;
        case NEQ:
            val2=MEM[SP--];
            val1=MEM[SP--];
            MEM[++SP]=(val1!=val2); 
            pc++;
            break;
        case GEQ:
            val2=MEM[SP--];
            val1=MEM[SP--];
            MEM[++SP]=(val1>=val2); 
            pc++;
            break;
        case GTR:
            val2=MEM[SP--];
            val1=MEM[SP--];
            MEM[++SP]=(val1>val2); 
            pc++;
            break;
        case LSS:
            val2=MEM[SP--];
            val1=MEM[SP--];
            MEM[++SP]=(val1<val2); 
            pc++;
            break;
        case BZE:
            if (MEM[SP--]==0) pc=INST.suite;
            else pc++; 
            break;
        case BRN:
            pc=INST.suite; 
            break;
        case PRN:
            val1 = MEM[SP--];
            printf("%d\n", val1);
            pc++;
            break;
        case INN:
            scanf("%d", &val1);
            adr = MEM[SP--];
            MEM[adr] = val1;
            pc++;
            break;
        case ADD:
            val1 = MEM[SP--];
            val2 = MEM[SP--];
            MEM[++SP] = val1 + val2;
            pc++;
            break;
        case SUB:
            val1 = MEM[SP--];
            val2 = MEM[SP--];
            MEM[++SP] = val1 - val2;
            pc++;
            break;
        case MUL:
            val1 = MEM[SP--];
            val2 = MEM[SP--];
            MEM[++SP] = val1 * val2;
            pc++;
            break;
        case DIV : 
            val1 = MEM[SP--];
            val2 = MEM[SP--];
            MEM[++SP] = val1 / val2;
            pc++;
            break;
 }
 }

 void INTER_PCODE(){
    pc = 0;
    while (PCODE[pc].MNE!=HLT){
        INTER_INST(PCODE[pc]);
    }

 }


void generer1(MNEMONIQUES mne){
    if (pc == TAILLECODE){
        printf("Erreur : Code trop long");
        exit(EXIT_FAILURE);
    }
    pc++;
    PCODE[pc].MNE = mne;
}

void generer2(MNEMONIQUES mne, int suite){
    if (pc == TAILLECODE){
        printf("Erreur : Code trop long");
        exit(EXIT_FAILURE);
    }
    pc++;
    PCODE[pc].MNE = mne;
    PCODE[pc].suite = suite;
}


void lire_mot()
{
    char mot[20];
    int indice = 0;
    mot[indice++] = Car_Cour;
    Lire_Car();
    while (isalpha(Car_Cour) || isdigit(Car_Cour))
    {
        mot[indice++] = Car_Cour;
        Lire_Car();
    }
    mot[indice] = '\0';
    if (stricmp(mot, "program") == 0)
    {
        SYM_COUR.CODE = PROGRAM_TOKEN;
    }
    else if (stricmp(mot, "const") == 0)
    {
        SYM_COUR.CODE = CONST_TOKEN;
    }
    else if (stricmp(mot, "var") == 0)
    {
        SYM_COUR.CODE = VAR_TOKEN;
    }
    else if (stricmp(mot, "begin") == 0)
    {
        SYM_COUR.CODE = BEGIN_TOKEN;
    }
    else if (stricmp(mot, "end") == 0)
    {
        SYM_COUR.CODE = END_TOKEN;
    }
    else if (stricmp(mot, "if") == 0)
    {
        SYM_COUR.CODE = IF_TOKEN;
    }
    else if (stricmp(mot, "then") == 0)
    {
        SYM_COUR.CODE = THEN_TOKEN;
    }
    else if (stricmp(mot, "while") == 0)
    {
        SYM_COUR.CODE = WHILE_TOKEN;
    }
    else if (stricmp(mot, "do") == 0)
    {
        SYM_COUR.CODE = DO_TOKEN;
    }
    else if (stricmp(mot, "read") == 0)
    {
        SYM_COUR.CODE = READ_TOKEN;
    }
    else if (stricmp(mot, "write") == 0)
    {
        SYM_COUR.CODE = WRITE_TOKEN;
    }
    else if (stricmp(mot, "else") == 0)
    {
        SYM_COUR.CODE = ELSE_TOKEN;
    }
    else if (stricmp(mot, "repeat") == 0)
    {
        SYM_COUR.CODE = REPEAT_TOKEN;
    }
    else if (stricmp(mot, "until") == 0)
    {
        SYM_COUR.CODE = UNTIL_TOKEN;
    }
    else if (stricmp(mot, "for") == 0)
    {
        SYM_COUR.CODE = FOR_TOKEN;
    }
    else if (stricmp(mot, "case") == 0)
    {
        SYM_COUR.CODE = CASE_TOKEN;
    }
    else if (stricmp(mot, "of") == 0)
    {
        SYM_COUR.CODE = OF_TOKEN;
    }
    else
    {
        SYM_COUR.CODE = ID_TOKEN;
    }
    strcpy(SYM_COUR.NOM, mot);
}

void lire_nombre()
{
    char nombre[11];
    int indice = 0;
    nombre[indice++] = Car_Cour;
    Lire_Car();
    while (isdigit(Car_Cour))
    {
        nombre[indice++] = Car_Cour;
        Lire_Car();
    }
    nombre[indice] = '\0';
    SYM_COUR.CODE = NUM_TOKEN;
    strcpy(SYM_COUR.NOM, nombre);
    SYM_COUR.val = atoi(SYM_COUR.NOM);
}

void Sym_Suiv()
{
    while (Car_Cour == ' ' || Car_Cour == '\n' || Car_Cour == '\t')
    {
        Lire_Car();
    }
    if (isalpha(Car_Cour))
    {
        lire_mot();
    }
    else if (isdigit(Car_Cour))
    {
        lire_nombre();
    }
    else
    {
        switch (Car_Cour)
        {
        case ';':
            SYM_COUR.CODE = PV_TOKEN;
            SYM_COUR.NOM[0] = ';';
            SYM_COUR.NOM[1] = '\0';
            Lire_Car();
            break;

        case '+':
            SYM_COUR.CODE = PLUS_TOKEN;
            SYM_COUR.NOM[0] = '+';
            SYM_COUR.NOM[1] = '\0';
            Lire_Car();
            break;

        case '-':
            SYM_COUR.CODE = MOINS_TOKEN;
            SYM_COUR.NOM[0] = '-';
            SYM_COUR.NOM[1] = '\0';
            Lire_Car();
            break;

        case '*':
            SYM_COUR.CODE = MULT_TOKEN;
            SYM_COUR.NOM[0] = '*';
            SYM_COUR.NOM[1] = '\0';
            Lire_Car();
            break;

        case '/':
            SYM_COUR.CODE = DIV_TOKEN;
            SYM_COUR.NOM[0] = '/';
            SYM_COUR.NOM[1] = '\0';
            Lire_Car();
            break;

        case ',':
            SYM_COUR.CODE = VIR_TOKEN;
            SYM_COUR.NOM[0] = ',';
            SYM_COUR.NOM[1] = '\0';
            Lire_Car();
            break;

        case ':':
            Lire_Car();
            if (Car_Cour == '=')
            {
                SYM_COUR.CODE = AFF_TOKEN;
                SYM_COUR.NOM[0] = ':';
                SYM_COUR.NOM[1] = '=';
                SYM_COUR.NOM[2] = '\0';
                Lire_Car();
            }
            else
            {
                SYM_COUR.CODE = DDOT_TOKEN;
            }
            break;

        case '<':
            SYM_COUR.NOM[0] = '<';
            Lire_Car();
            if (Car_Cour == '=')
            {
                SYM_COUR.CODE = INFEG_TOKEN;
                
                SYM_COUR.NOM[1] = '=';
                SYM_COUR.NOM[2] = '\0';
                Lire_Car();
            }
            else if (Car_Cour == '>')
            {
                SYM_COUR.CODE = DIFF_TOKEN;
                SYM_COUR.NOM[1] = '>';
                SYM_COUR.NOM[2] = '\0';
                Lire_Car();
            }
            else
            {
                SYM_COUR.CODE = INF_TOKEN;
                SYM_COUR.NOM[1] = '\0';
            }
            break;

        case '>':
            Lire_Car();
            SYM_COUR.NOM[0] = '>';
            if (Car_Cour == '=')
            {
                SYM_COUR.CODE = SUPEG_TOKEN;
                SYM_COUR.NOM[1] = '=';
                SYM_COUR.NOM[2] = '\0';
                Lire_Car();
            }
            else
            {
                SYM_COUR.CODE = SUP_TOKEN;
            }
            break;

        case '(':
            SYM_COUR.CODE = PO_TOKEN;
            SYM_COUR.NOM[0] = '(';
            SYM_COUR.NOM[1] = '\0';
            Lire_Car();
            break;
        case '=':
            SYM_COUR.CODE = EG_TOKEN;
            SYM_COUR.NOM[0] = '=';
            SYM_COUR.NOM[1] = '\0';
            Lire_Car();
            break;

        case ')':
            SYM_COUR.CODE = PF_TOKEN;
            SYM_COUR.NOM[0] = ')';
            SYM_COUR.NOM[1] = '\0';
            Lire_Car();
            break;

        case '.':
            SYM_COUR.CODE = PT_TOKEN;
            SYM_COUR.NOM[0] = '.';
            SYM_COUR.NOM[1] = '\0';
            Lire_Car();
            break;

        case EOF:
            SYM_COUR.CODE = EOF_TOKEN;
            SYM_COUR.NOM[0] = EOF;
            SYM_COUR.NOM[1] = '\0';
            break;
        // case '{':
        //     Lire_Car();
        //     if(Car_Cour=='*'){
        //         Lire_Car();
        //     }

        default:
            SYM_COUR.CODE = ERREUR_TOKEN;
            SYM_COUR.NOM[0] = Car_Cour;
            SYM_COUR.NOM[1] = '\0';
            Lire_Car();
        }
        //strcpy(SYM_COUR.NOM , Car_Cour);
    }

    printf("Symbol: %s\n", SYM_COUR.NOM);
}

void Lire_Car()
{
    Car_Cour = fgetc(fichier);
}

void Erreur(CODES_ERR code)
{
    printf("Erreur: %d\n", code);
    printf("Current Token: %d\n", SYM_COUR.CODE);
    printf("Current Lexeme: %s\n", SYM_COUR.NOM);
    exit(EXIT_FAILURE);
}

void Test_Symbole(CODES_LEX cl, CODES_ERR COD_ERR)
{
    if (SYM_COUR.CODE == cl)
    {
        Sym_Suiv();
    }
    else
        Erreur(COD_ERR);
}

int idf_existe(char* nom){
    int existe = 0;
    for (int i=0; i<nbreIdfs; i++){
        if (strcmpi(TAB_IDFS[i].NOM,nom)==0){
            existe = 1;
        }
    }
    return existe;
}


void ajouter_idf(char* nom, TSYM type){
    if (nbreIdfs == 0){
        strcpy(TAB_IDFS[nbreIdfs].NOM,nom);
        TAB_IDFS[nbreIdfs].TIDF = type;
        TAB_IDFS[nbreIdfs].adresse = ++offset;
        nbreIdfs++;
    }else{
        int existe = 0;
        for (int i=0; i<nbreIdfs; i++){
            if (strcmpi(TAB_IDFS[i].NOM,nom)==0){
                existe = 1;
                if (TAB_IDFS[i].TIDF==TPROG){
                    printf("Erreur sémantique à %s : Le ID du programme ne peut être utilisé dans le programme", nom);
                }else{
                    printf("Erreur sémantique à %s : Double déclaration", nom);
                }
                exit(EXIT_FAILURE);
            }
        };
        if (!existe){
            strcpy(TAB_IDFS[nbreIdfs].NOM,nom);
            TAB_IDFS[nbreIdfs].TIDF = type;
            TAB_IDFS[nbreIdfs].adresse = ++offset;
            nbreIdfs++;
        }
    }
}

void PROGRAM()
{
    Test_Symbole(PROGRAM_TOKEN, PROGRAM_ERR);
    //Test_Symbole(ID_TOKEN, ID_ERR);
    if (SYM_COUR.CODE == ID_TOKEN){
        ajouter_idf(SYM_COUR.NOM, TPROG);
        Sym_Suiv();
    }else{
        Erreur(ID_ERR);
    }
    Test_Symbole(PV_TOKEN, PV_ERR);
    BLOCK();
    //génération de fin de programme
    generer1(HLT);
    Test_Symbole(PT_TOKEN, PT_ERR);

}

void BLOCK()
{
    CONSTS();
    VARS();
    // génération du début de programme
    PCODE[0].MNE = INT;
    PCODE[0].suite = offset;
    INSTS();
}

void CONSTS()
{
    switch (SYM_COUR.CODE)
    {
    case CONST_TOKEN:
        Sym_Suiv();
        //Test_Symbole(ID_TOKEN, ID_ERR);
        if (SYM_COUR.CODE == ID_TOKEN){
            ajouter_idf(SYM_COUR.NOM, TCONST);
            generer2(LDA, TAB_IDFS[nbreIdfs-1].adresse);
            Sym_Suiv();
        }else{
            Erreur(ID_ERR);
        }
        Test_Symbole(EG_TOKEN, EG_ERR);
        //Test_Symbole(NUM_TOKEN, NUM_ERR);
        if (SYM_COUR.CODE == NUM_TOKEN){
            TAB_IDFS[nbreIdfs-1].val = SYM_COUR.val;
            generer2(LDI, SYM_COUR.val);
            Sym_Suiv();
        }else Erreur(NUM_ERR);
        Test_Symbole(PV_TOKEN, PV_ERR);
        generer1(STO);
        while (SYM_COUR.CODE == ID_TOKEN)
        {
            ajouter_idf(SYM_COUR.NOM, TCONST);
            generer2(LDA, TAB_IDFS[nbreIdfs-1].adresse);
            Sym_Suiv();
            Test_Symbole(EG_TOKEN, EG_ERR);
            //Test_Symbole(NUM_TOKEN, NUM_ERR);
             if (SYM_COUR.CODE == NUM_TOKEN){
                TAB_IDFS[nbreIdfs-1].val = SYM_COUR.val;
                generer2(LDI, SYM_COUR.val);
                Sym_Suiv();
            }else Erreur(NUM_ERR);
            Test_Symbole(PV_TOKEN, PV_ERR);
            generer1(STO);
        };
        break;
    case VAR_TOKEN:
        break;
    case BEGIN_TOKEN:
        break;
    default:
        Erreur(CONST_VAR_BEGIN_ERR);
        break;
    }
}

void VARS()
{
    switch (SYM_COUR.CODE)
    {
    case VAR_TOKEN:
        Sym_Suiv();
        //Test_Symbole(ID_TOKEN, ID_ERR);
        if (SYM_COUR.CODE == ID_TOKEN){
            ajouter_idf(SYM_COUR.NOM, TVAR);
            Sym_Suiv();
        }else{
            Erreur(ID_ERR);
        }
        while (SYM_COUR.CODE == VIR_TOKEN)
        {
            Sym_Suiv();
            //Test_Symbole(ID_TOKEN, ID_ERR);
            if (SYM_COUR.CODE == ID_TOKEN){
            ajouter_idf(SYM_COUR.NOM, TVAR);
            Sym_Suiv();
            }else{
                Erreur(ID_ERR);
            }
        }
        Test_Symbole(PV_TOKEN, PV_ERR);
        break;
    case BEGIN_TOKEN:
        break;
    default:
        Erreur(VAR_BEGIN_ERR);
        break;
    }
}

void INSTS()
{
    //begin INST { ; INST } end
    if (SYM_COUR.CODE == BEGIN_TOKEN)
    {
        Sym_Suiv();
        INST();
        while (SYM_COUR.CODE == PV_TOKEN)
        {
            Sym_Suiv();
            INST();
        }

        if (SYM_COUR.CODE == END_TOKEN)
        {
            Sym_Suiv();
        }
        else
        {
            Erreur(FIN_ERR);
        }
    }
    else
    {
        Erreur(BEGIN_ERR);
    }
}

void INST()

{
    //INSTS | AFFEC | SI | TANTQUE | ECRIRE | LIRE | e
    switch (SYM_COUR.CODE)
    {
    case BEGIN_TOKEN:
        INSTS();
        break;
    case ID_TOKEN:
        AFFEC();
        break;
    case IF_TOKEN:
        SI();
        break;
    case WHILE_TOKEN:
        TANTQUE();
        break;
    case WRITE_TOKEN:
        ECRIRE();
        break;
    case READ_TOKEN:
        LIRE();
        break;
    case REPEAT_TOKEN:
        REPETER();
        break;
    case FOR_TOKEN:
        POUR();
        break;
    case CASE_TOKEN:
        CAS();
        break;
    default:
        break;
    }
}

void AFFEC()
{
    //ID := EXPR
    //Test_Symbole(ID_TOKEN, ID_ERR);
    if (SYM_COUR.CODE==ID_TOKEN){
        for (int i=0; i<nbreIdfs; i++){
            if (strcmpi(TAB_IDFS[i].NOM, SYM_COUR.NOM)==0){
                generer2(LDA, TAB_IDFS[i].adresse);
            }
        }
        Sym_Suiv();
    }else Erreur(ID_ERR);
    Test_Symbole(AFF_TOKEN, AFF_ERR);
    EXPR();
    generer1(STO);
}

void SI()
{
    Test_Symbole(IF_TOKEN, IF_ERR);
    COND();
    Test_Symbole(THEN_TOKEN, THEN_ERR);
    int brn = ++ pc;
    PCODE[brn].MNE = BZE;
    INST();
    PCODE[brn].suite = pc + 1;
    if (SYM_COUR.CODE == ELSE_TOKEN)
    {
        Sym_Suiv();
        INST();
    }
}

void TANTQUE()
{
    Test_Symbole(WHILE_TOKEN, WHILE_ERR);
    int LABEL_BRN = pc +1;
    COND();
    generer1(BZE);
    int INDICE_BZE = pc;
    Test_Symbole(DO_TOKEN, DO_ERR);
    INST();
    generer2(BRN, LABEL_BRN);
    PCODE[INDICE_BZE].suite = pc +1;
}

void ECRIRE()
{
    Test_Symbole(WRITE_TOKEN, WRITE_ERR);
    Test_Symbole(PO_TOKEN, PO_ERR);
    EXPR();
    generer1(PRN);
    while (SYM_COUR.CODE == VIR_TOKEN)
    {
        Sym_Suiv();
        EXPR();
        generer1(PRN);
    }

    Test_Symbole(PF_TOKEN, PF_ERR);
}

void LIRE()
{
    Test_Symbole(READ_TOKEN, READ_ERR);
    Test_Symbole(PO_TOKEN, PO_ERR);
    //Test_Symbole(ID_TOKEN, ID_ERR);
    if (SYM_COUR.CODE == ID_TOKEN){
        for (int i=0; i<nbreIdfs; i++){
            if (strcmpi(SYM_COUR.NOM, TAB_IDFS[i].NOM)==0){
                generer2(LDA, TAB_IDFS[i].adresse);
                generer1(INN);
            }
        }
        Sym_Suiv();
    }else Erreur(ID_ERR);

    while (SYM_COUR.CODE == VIR_TOKEN)
    {
        Sym_Suiv();
        //Test_Symbole(ID_TOKEN, ID_ERR);
        if (SYM_COUR.CODE == ID_TOKEN){
            for (int i=0; i<nbreIdfs; i++){
                if (strcmpi(SYM_COUR.NOM, TAB_IDFS[i].NOM)==0){
                    generer2(LDA, TAB_IDFS[i].adresse);
                    generer1(INN);
                }
            }
            Sym_Suiv();
        }else Erreur(ID_ERR);

    }

    Test_Symbole(PF_TOKEN, PF_ERR);
}

void COND()
{
    EXPR();
    int op = SYM_COUR.CODE;
    RELOP();
    MNEMONIQUES relop;
    switch (op){
        case EG_TOKEN : relop = EQL; break;
        case INFEG_TOKEN : relop = LEQ; break;
        case DIFF_TOKEN : relop  = NEQ; break;
        case INF_TOKEN : relop  = LSS; break;
        case SUP_TOKEN : relop  = GTR; break;
        case SUPEG_TOKEN : relop  = GEQ; break;
        default : break;
    }
    EXPR();
    generer1(relop);
}

void EXPR()
{
    //TERM { ADDOP TERM }
    TERM();
    while (SYM_COUR.CODE == PLUS_TOKEN || SYM_COUR.CODE == MOINS_TOKEN)
    {
        MNEMONIQUES  OP;
        if (SYM_COUR.CODE == PLUS_TOKEN) OP = ADD;
        else OP = SUB;
        ADDOP();
        TERM();
        generer1(OP);
    }
}

void TERM()
{
    FACT();
    while (SYM_COUR.CODE == MULT_TOKEN || SYM_COUR.CODE == DIV_TOKEN)
    {
        MNEMONIQUES OP;
        if (SYM_COUR.CODE==MULT_TOKEN) OP = MUL;
        else OP = DIV;
        MULOP();
        FACT();
        generer1(OP);
    }
}

void FACT()
{
    switch (SYM_COUR.CODE)
    {
    case ID_TOKEN:
        for(int i=0; i<nbreIdfs; i++){
            if (strcmpi(SYM_COUR.NOM,TAB_IDFS[i].NOM)==0){
                generer2(LDA, TAB_IDFS[i].adresse);
                generer1(LDV);
            }
        }
        Test_Symbole(ID_TOKEN, ID_ERR);
        break;
    case NUM_TOKEN:
        generer2(LDI, SYM_COUR.val);
        Test_Symbole(NUM_TOKEN, NUM_ERR);
        break;
    case PO_TOKEN:
        Test_Symbole(PO_TOKEN, PO_ERR);
        EXPR();
        Test_Symbole(PF_TOKEN, PF_ERR);
        break;
    default:
        Erreur(ERREUR_ERR);
        break;
    }
}

void RELOP()
{
    switch (SYM_COUR.CODE)
    {
    case EG_TOKEN:
        Test_Symbole(EG_TOKEN, EG_ERR);
        break;
    case DIFF_TOKEN:
        Test_Symbole(DIFF_TOKEN, DIFF_ERR);
        break;
    case INF_TOKEN:
        Test_Symbole(INF_TOKEN, INF_ERR);
        break;
    case SUP_TOKEN:
        Test_Symbole(SUP_TOKEN, SUP_ERR);
        break;
    case INFEG_TOKEN:
        Test_Symbole(INFEG_TOKEN, INFEG_ERR);
        break;
    case SUPEG_TOKEN:
        Test_Symbole(SUPEG_TOKEN, SUPEG_ERR);
        break;
    default:
        Erreur(ERREUR_ERR);
        break;
    }
}

void ADDOP()
{
    switch (SYM_COUR.CODE)
    {
    case PLUS_TOKEN:
        Test_Symbole(PLUS_TOKEN, PLUS_ERR);
        break;
    case MOINS_TOKEN:
        Test_Symbole(MOINS_TOKEN, MOINS_ERR);
        break;
    default:
        Erreur(ERREUR_ERR);
        break;
    }
}

void MULOP()
{
    switch (SYM_COUR.CODE)
    {
    case MULT_TOKEN:
        Test_Symbole(MULT_TOKEN, MULT_ERR);
        break;
    case DIV_TOKEN:
        Test_Symbole(DIV_TOKEN, DIV_ERR);
        break;
    default:
        Erreur(ERREUR_ERR);
        break;
    }
}

void POUR()
{
    Test_Symbole(FOR_TOKEN, FOR_ERR);
    Test_Symbole(ID_TOKEN, ID_ERR);
    Test_Symbole(AFF_TOKEN, AFF_ERR);

    switch (SYM_COUR.CODE)
    {
    case DOWNTO_TOKEN:
        Test_Symbole(DOWNTO_TOKEN, DOWNTO_ERR);
        break;
    case INTO_TOKEN:
        Test_Symbole(INTO_TOKEN, INTO_ERR);
        break;
    default:
        Erreur(ERREUR_ERR);
        break;
    }
    Test_Symbole(NUM_TOKEN, NUM_ERR);
    Test_Symbole(DO_TOKEN, DO_ERR);
    INST();

}


void REPETER(){
    Test_Symbole(REPEAT_TOKEN, REPEAT_ERR);
    int IND_BZE = pc +1;
    INST();
    Test_Symbole(UNTIL_TOKEN, UNTIL_ERR);
    COND();
    generer2(BZE, IND_BZE);
}

void CAS()
{
    Test_Symbole(CASE_TOKEN, CASE_ERR);
    Test_Symbole(ID_TOKEN, ID_ERR);
    Test_Symbole(OF_TOKEN, OF_TOKEN);
    Test_Symbole(NUM_TOKEN, NUM_ERR);
    Test_Symbole(DDOT_TOKEN, DDOT_ERR);
    INST();
     while (SYM_COUR.CODE == NUM_TOKEN)
    {
        Sym_Suiv();
        Test_Symbole(DDOT_TOKEN, DDOT_ERR);
        INST();
    }
    if (SYM_COUR.CODE == ELSE_TOKEN) {
        Sym_Suiv();
        INST();
    }
    
    Test_Symbole(END_TOKEN, END_ERR);
}

void regle3(){
    fichier = fopen("program.p", "r");
    Lire_Car();
    Sym_Suiv();
    //printf("Le symbole courant : %s", SYM_COUR.NOM);
    while(SYM_COUR.CODE!=BEGIN_TOKEN) Sym_Suiv();
    while(SYM_COUR.CODE!=EOF_TOKEN){
        if (SYM_COUR.CODE == ID_TOKEN){
            if (!idf_existe(SYM_COUR.NOM)){
                printf("Erreur sémantique à %s : variable non déclarée.", SYM_COUR.NOM);
                exit(EXIT_FAILURE);
            }
        }
        Sym_Suiv();
    }
    return;
}

void regle4(){
    fichier = fopen("program.p", "r");
    Lire_Car();
    while(SYM_COUR.CODE != BEGIN_TOKEN) Sym_Suiv();
    while(SYM_COUR.CODE != EOF_TOKEN){
        if (SYM_COUR.CODE == ID_TOKEN){
            for (int i=0; i<nbreIdfs; i++){
                if ((strcmpi(SYM_COUR.NOM, TAB_IDFS[i].NOM) == 0) && TAB_IDFS[i].TIDF==TCONST){
                    Sym_Suiv();
                    if (SYM_COUR.CODE==AFF_TOKEN){
                        Sym_Suiv();
                        if (TAB_IDFS[i].val != SYM_COUR.val){
                            printf("Erreur sémantique : une constante ne peut pas changer de valeur.");
                            exit(EXIT_FAILURE);
                        }
                    }

                }
            }
        }
        if (SYM_COUR.CODE == READ_TOKEN){
            Sym_Suiv();
            Sym_Suiv();
            if (SYM_COUR.CODE == ID_TOKEN){
                for (int i = 0; i<nbreIdfs; i++){
                    if ((strcmpi(TAB_IDFS[i].NOM, SYM_COUR.NOM)==0) && TAB_IDFS[i].TIDF==TCONST){
                        printf("Erreur sémantique : une constante ne peut pas changer de valeur.");
                        exit(EXIT_FAILURE);
                    }
                }
            }
        }
        Sym_Suiv();
    }
    return;
}

void regle5(){
    fichier = fopen("program.p", "r");
    Lire_Car();
    while (SYM_COUR.CODE!=BEGIN_TOKEN) Sym_Suiv();
    while (SYM_COUR.CODE!=EOF_TOKEN){
        if (SYM_COUR.CODE==ID_TOKEN){
            for (int i=0; i<nbreIdfs; i++){
                if ((strcmpi(TAB_IDFS[i].NOM,SYM_COUR.NOM)==0)&& TAB_IDFS[i].TIDF==TPROG){
                    printf("Erreur sémantique à %s :  Le ID du programme ne peut être utilisé dans le programme.",SYM_COUR.NOM);
                    exit(EXIT_FAILURE);
                }
            }
        }
        Sym_Suiv();
    }
    return;
}

void semantique(){
    regle3();
    //printf("Règle 3 terminée");
    regle4();
    //printf("Règle 4 terminée");
    regle5();
    //printf("Analyse sémantique réussie.");
}

int main()
{
    fichier = fopen("program.p", "r");
    FICH_SORTIE = fopen("sortie3.op","w+");
    if (fichier == NULL)
    {
        perror("Erreur lors de l'ouverture du fichier");
        return 1;
    }

    // PREMIER_SYM();
    Lire_Car();
    Sym_Suiv();

    PROGRAM();
    printf("Analyse syntaxique terminee.\n", SYM_COUR.NOM);
    semantique();
    printf("Analyse semantique terminee");

    //printf("Program execution completed.\n");

    if (SYM_COUR.CODE == EOF_TOKEN)
    {
        printf("BRAVO: le programme est correcte!!!\n");
    }
    else
    {
        printf("PAS BRAVO: fin de programme erronée!!!!\n");
        
        printf("Current Token: %s\n", afficherToken(SYM_COUR.CODE));
        printf("Current Lexeme: %s\n", SYM_COUR.NOM);
        Sym_Suiv(); // Move this line inside the else block
    }
    SavePCodeToFile();
    fclose(FICH_SORTIE);
    fclose(fichier);
    INTER_PCODE();
    return 0;
}
