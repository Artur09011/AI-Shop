#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_PRODUCTS 50
#define MAX_PRODUCT_NAME_LEN 50
#define MAX_ORDER_LEN 1000

// Structure to hold product details
typedef struct {
    char name[MAX_PRODUCT_NAME_LEN];
    float price;
    int quantity;
} Product;

// Structure to hold order details
typedef struct {
    char firstName[50];
    char lastName[50];
    char address[100];
    char coffeeName[MAX_PRODUCT_NAME_LEN];
    int quantity;
    float totalPrice;
} Order;

// Function to load products from file
int loadProducts(Product products[]) {
    FILE* file = fopen("products.txt", "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return 0;
    }

    int count = 0;
    while (fscanf(file, "%s %f %d", products[count].name, &products[count].price, &products[count].quantity) != EOF) {
        count++;
    }

    fclose(file);
    return count;
}

// Function to display available products
void displayProducts(Product products[], int count) {
    printf("Available Products:\n");
    for (int i = 0; i < count; i++) {
        printf("%d. %s - $%.2f - Quantity: %d\n", i + 1, products[i].name, products[i].price, products[i].quantity);
    }
}

// Function to update products in the file
void updateProductsFile(Product products[], int count) {
    FILE* file = fopen("products.txt", "w");
    if (file == NULL) {
        printf("Error updating file.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%s %.2f %d\n", products[i].name, products[i].price, products[i].quantity);
    }

    fclose(file);
}

// Function to save order details in orders.txt
void saveOrder(Order order) {
    FILE* file = fopen("orders.txt", "a");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    fprintf(file, "Customer Name: %s %s\n", order.firstName, order.lastName);
    fprintf(file, "Address: %s\n", order.address);
    fprintf(file, "Coffee Ordered: %s\n", order.coffeeName);
    fprintf(file, "Quantity: %d\n", order.quantity);
    fprintf(file, "Total Price: $%.2f\n\n", order.totalPrice);

    fclose(file);
}

int main() {
    Product availableProducts[MAX_PRODUCTS];
    int productsCount = loadProducts(availableProducts);

    if (productsCount == 0) {
        printf("No products available.\n");
        return 1;
    }

    char address[100], firstName[50], lastName[50], coffeeName[MAX_PRODUCT_NAME_LEN];
    int quantity, minutes;
    int exitProgram = 0; // Variable to control program exit

    do {
        printf("Enter your first name: ");
        scanf("%s", firstName);

        printf("Enter your last name: ");
        scanf("%s", lastName);

        printf("Enter your address: ");
        scanf(" %[^\n]s", address);

        displayProducts(availableProducts, productsCount);

        printf("Enter the coffee you want to buy (or type 'exit' to quit): ");
        scanf("%s", coffeeName);

        if (strcmp(coffeeName, "exit") == 0) {
            exitProgram = 1; // Set the exit flag if user wants to exit
        }
        else {
            int found = 0;
            float price = 0.0;

            for (int i = 0; i < productsCount; i++) {
                if (strcmp(coffeeName, availableProducts[i].name) == 0) {
                    found = 1;
                    if (availableProducts[i].quantity > 0) {
                        printf("Enter the quantity: ");
                        scanf("%d", &quantity);

                        if (quantity > availableProducts[i].quantity) {
                            printf("Sorry, the requested quantity is not available.\n");
                            break;
                        }

                        printf("Enter how many minutes until you'll take your coffee: ");
                        scanf("%d", &minutes);

                        price = quantity * availableProducts[i].price;
                        availableProducts[i].quantity -= quantity; // Update available quantity
                        printf("The total cost is: $%.2f\n", price);

                        // Save order details in orders.txt
                        Order newOrder;
                        strcpy(newOrder.firstName, firstName);
                        strcpy(newOrder.lastName, lastName);
                        strcpy(newOrder.address, address);
                        strcpy(newOrder.coffeeName, coffeeName);
                        newOrder.quantity = quantity;
                        newOrder.totalPrice = price;
                        saveOrder(newOrder);
                    }
                    else {
                        printf("Sorry, the selected coffee is currently out of stock.\n");
                        printf("Your coffee will be ready in %d minutes.\n", (rand() % 10) + 1); // Random time between 1 to 10 minutes
                    }
                    break;
                }
            }

            if (!found) {
                printf("Sorry, the entered coffee is not available.\n");
            }
        }
    } while (!exitProgram);

    // Update the products.txt file with the new quantities
    updateProductsFile(availableProducts, productsCount);

    return 0;
}
