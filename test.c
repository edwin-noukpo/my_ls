#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure pour représenter un nœud de la liste chaînée
typedef struct Node {
    char *word;
    struct Node *next;
} Node;

// Fonction pour créer un nouveau nœud
Node* create_node(char *word) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->word = strdup(word); // Copie du mot
    new_node->next = NULL;
    return new_node;
}

// Fonction pour insérer un mot dans la liste de manière triée
void insert_sorted(Node **head, char *word) {
    Node *new_node = create_node(word);

    // Cas où la liste est vide ou où le nouveau mot doit être placé au début
    if (*head == NULL || strcmp((*head)->word, word) > 0) {
        new_node->next = *head;
        *head = new_node;
    } else {
        // Trouver la position d'insertion correcte
        Node *current = *head;
        while (current->next != NULL && strcmp(current->next->word, word) < 0) {
            current = current->next;
        }
        new_node->next = current->next;
        current->next = new_node;
    }
}

// Fonction pour afficher la liste chaînée
void display_list(Node *head) {
    Node *current = head;
    while (current != NULL) {
        printf("%s\n", current->word);
        current = current->next;
    }
}

// Fonction pour libérer la mémoire utilisée par la liste chaînée
void free_list(Node *head) {
    Node *current = head;
    Node *next_node;
    while (current != NULL) {
        next_node = current->next;
        free(current->word); // Libère la mémoire allouée pour le mot
        free(current);       // Libère la mémoire allouée pour le nœud
        current = next_node;
    }
}

int main() {
    Node *head = NULL;
    int n;

    // Demander le nombre de mots
    printf("Combien de mots voulez-vous entrer ? ");
    scanf("%d", &n);

    // Lire chaque mot et l'insérer dans la liste triée
    for (int i = 0; i < n; i++) {
        char word[100]; // suppose que chaque mot fait au maximum 100 caractères
        printf("Entrez le mot %d : ", i + 1);
        scanf("%s", word);
        insert_sorted(&head, word);
    }

    // Afficher les mots triés
    printf("\nMots triés :\n");
    display_list(head);

    // Libérer la mémoire allouée
    free_list(head);

    return 0;
}
