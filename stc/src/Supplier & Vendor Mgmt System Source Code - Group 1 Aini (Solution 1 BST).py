import numpy as np
import pandas as pd
import time
import os


# Define Supplier class to store supplier information
class Supplier:
    def __init__(self, supplier_id, supplier_name, supplier_contact, supplier_product, supplier_rating):
        self.supplier_id = supplier_id  # Unique ID for the supplier
        self.supplier_name = supplier_name  # Supplier company name
        self.supplier_contact = supplier_contact  # Contact information
        self.supplier_product = supplier_product  # Type of product supplied
        self.supplier_rating = supplier_rating  # Quality rating (1-5)

    # Display supplier's information in formatted way
    def display_info(self):
        print(f"Supplier ID: {self.supplier_id}")
        print(f"Supplier Name: {self.supplier_name}")
        print(f"Contact: {self.supplier_contact}")
        print(f"Supplier Product: {self.supplier_product}")
        print(f"Supplier Rating: {self.supplier_rating}")


# Define BST Node class for Binary Search Tree
class BSTNode:
    def __init__(self, supplier):
        self.supplier = supplier  # Store supplier object
        self.left = None  # Pointer to left child (smaller ID)
        self.right = None  # Pointer to right child (larger ID)

    # Insert new supplier into BST based on ID
    def insert(self, supplier):
        if supplier.supplier_id < self.supplier.supplier_id:
            if self.left is None:
                self.left = BSTNode(supplier)
            else:
                self.left.insert(supplier)
        else:
            if self.right is None:
                self.right = BSTNode(supplier)
            else:
                self.right.insert(supplier)

    # Search for supplier by ID in BST
    def search(self, supplier_id):
        if supplier_id == self.supplier.supplier_id:
            return self.supplier
        elif supplier_id < self.supplier.supplier_id and self.left:
            return self.left.search(supplier_id)
        elif supplier_id > self.supplier.supplier_id and self.right:
            return self.right.search(supplier_id)
        else:
            return None

    # Get all suppliers in sorted order (in-order traversal)
    def display_order(self):
        supplier_list = []
        
        if self.left:
            supplier_list.extend(self.left.display_order())
            
        supplier_list.append(self.supplier)
        
        if self.right:
            supplier_list.extend(self.right.display_order())
            
        return supplier_list

    # Delete supplier from BST
    def delete(self, supplier_id):
        if supplier_id < self.supplier.supplier_id:
            if self.left:
                self.left = self.left.delete(supplier_id)
        elif supplier_id > self.supplier.supplier_id:
            if self.right:
                self.right = self.right.delete(supplier_id)
        else:
            # Node to be deleted found
            if self.left is None:
                return self.right
            elif self.right is None:
                return self.left
            
            # Node with two children: get inorder successor
            temp = self.right
            while temp.left:
                temp = temp.left
            self.supplier = temp.supplier
            self.right = self.right.delete(temp.supplier.supplier_id)
            
        return self


# Main management system class
class SupplierVendorManagementSystem:
    def __init__(self):
        self.root = None  # Root of the BST

    # Check if supplier ID already exists
    def supplier_exists(self, supplier_id):
        if self.root is None:
            return False
        return self.root.search(supplier_id) is not None

    # Add new supplier to the system with timing
    def add_supplier(self, supplier_id, supplier_name, supplier_contact, supplier_product, supplier_rating):
        start_time = time.time()
        
        # Check for duplicate ID
        if self.supplier_exists(supplier_id):
            execution_time = (time.time() - start_time) * 1000000
            print(f"Error: Supplier ID {supplier_id} already exists! (Time: {execution_time:.2f}μs)")
            return False
            
        new_supplier = Supplier(supplier_id, supplier_name, supplier_contact, supplier_product, supplier_rating)

        if self.root is None:
            self.root = BSTNode(new_supplier)
            execution_time = (time.time() - start_time) * 1000000
            print(f"Supplier added successfully as root! (Time: {execution_time:.2f}μs)")
        else:
            self.root.insert(new_supplier)
            execution_time = (time.time() - start_time) * 1000000
            print(f"Supplier added successfully! (Time: {execution_time:.2f}μs)")
        return True

    # Search for supplier by ID with timing
    def search_supplier(self, supplier_id):
        start_time = time.time()
        
        if self.root is None:
            execution_time = (time.time() - start_time) * 1000000
            print(f"No suppliers in the system! (Time: {execution_time:.2f}μs)")
            return None
        else:
            supplier = self.root.search(supplier_id)
            execution_time = (time.time() - start_time) * 1000000
            
            if supplier:
                print(f"Supplier found (Time: {execution_time:.2f}μs):")
                print("-" * 40)
                supplier.display_info()
                print("-" * 40)
                return supplier
            else:
                print(f"Supplier with ID {supplier_id} not found! (Time: {execution_time:.2f}μs)")
                return None

    # Delete supplier by ID
    def delete_supplier(self, supplier_id):
        start_time = time.time()
        
        if self.root is None:
            execution_time = (time.time() - start_time) * 1000000
            print(f"No suppliers in the system! (Time: {execution_time:.2f}μs)")
            return False
        else:
            # Search first to confirm supplier exists
            supplier = self.root.search(supplier_id)
            if supplier:
                self.root = self.root.delete(supplier_id)
                execution_time = (time.time() - start_time) * 1000000
                print(f"Supplier with ID {supplier_id} deleted successfully! (Time: {execution_time:.2f}μs)")
                return True
            else:
                execution_time = (time.time() - start_time) * 1000000
                print(f"Supplier with ID {supplier_id} not found! (Time: {execution_time:.2f}μs)")
                return False

    # Update supplier information
    def update_supplier(self, supplier_id):
        start_time = time.time()
        
        if self.root is None:
            execution_time = (time.time() - start_time) * 1000000
            print(f"\nNo suppliers in the system! (Time: {execution_time:.2f}μs)")
            return False
        else:
            supplier = self.root.search(supplier_id)
            if supplier:
                print(f"Current supplier information:")
                print("-" * 40)
                supplier.display_info()
                print("-" * 40)
                
                # Get updated information
                print("\nEnter new information (press Enter to keep current value):\n")
                
                new_name = input(f"Supplier Name [{supplier.supplier_name}]: ").strip()
                new_contact = input(f"Contact [{supplier.supplier_contact}]: ").strip()
                new_product = input(f"Supplier Product [{supplier.supplier_product}]: ").strip()
                new_rating = input(f"Rating (1-5) [{supplier.supplier_rating}]: ").strip()
                
                # Update fields if new values provided
                if new_name:
                    supplier.supplier_name = new_name
                if new_contact:
                    supplier.supplier_contact = new_contact
                if new_product:
                    supplier.supplier_product = new_product
                if new_rating:
                    try:
                        rating = int(new_rating)
                        if 1 <= rating <= 5:
                            supplier.supplier_rating = rating
                        else:
                            print("\nRating must be between 1-5! Keeping current rating.")
                    except ValueError:
                        print("\nInvalid rating! Keeping current rating.")
                
                execution_time = (time.time() - start_time) * 1000000
                print(f"\nSupplier information updated successfully! (Time: {execution_time:.2f}μs)")
                return True
            else:
                execution_time = (time.time() - start_time) * 1000000
                print(f"\nSupplier with ID {supplier_id} not found! (Time: {execution_time:.2f}μs)")
                return False

    # Display all suppliers in sorted order with timing
    def display_allsuppliers(self):
        start_time = time.time()
        
        if self.root is None:
            execution_time = (time.time() - start_time) * 1000000
            print(f"No suppliers in the system! (Time: {execution_time:.2f}μs)")
        else:
            print("\n" + "=" * 50)
            print("           ALL SUPPLIERS (SORTED BY ID)")
            print("=" * 50)
            suppliers = self.root.display_order()
            
            for supplier in suppliers:
                supplier.display_info()
                print("-" * 30)
                
            execution_time = (time.time() - start_time) * 1000000
            print(f"Total suppliers: {len(suppliers)} (Time: {execution_time:.2f}μs)")


# Create sample data for demonstration
def create_sampledata(system):
    sample_data = [
        ("S078", "VitaCo", "012-1234567", "Ascorbic Acid 1000mg", 5),
        ("S087", "LiquidRx", "014-1234567", "Cough Syrup 100mL", 5),
        ("S022", "GlucoSafe", "015-1234567", "Insulin Pen U-100", 4)
        ]

    for supplier_data in sample_data:
        system.add_supplier(*supplier_data)


# Clear screen function
def clear_screen():
    os.system('cls' if os.name == 'nt' else 'clear')


# Menu functions for each operation
def add_supplier_menu(system):
    clear_screen()
    print("      INSERT SUPPLIER INFORMATION")
    print("----------------------------------------")
    print()
    
    try:
        supplier_id = input("Enter Supplier ID: ")
        if system.supplier_exists(supplier_id):
            print(f"\nError! Supplier ID {supplier_id} already exists!")
            input("\nPress Enter to return to main menu...")
            return
        
        supplier_name = input("Enter Supplier Name: ")
        supplier_contact = input("Enter Contact Info: ")
        supplier_product = input("Enter Supplier Product: ")
        supplier_rating = int(input("Enter Rating (1-5): "))

        if 1 <= supplier_rating <= 5:
            system.add_supplier(supplier_id, supplier_name, supplier_contact, supplier_product, supplier_rating)
        else:
            print("Rating must be between 1-5!")
    except ValueError:
        print("\nInvalid input! Please enter the relevant information.")
    
    input("\nPress Enter to return to main menu...")


def search_supplier_menu(system):
    clear_screen()
    print("            SEARCH SUPPLIER")
    print("----------------------------------------")
    print()
    
    try:
        supplier_id = input("Enter Supplier ID to search: ")
        print()
        system.search_supplier(supplier_id)
    except ValueError:
        print("\nInvalid input! Please enter a numeric Supplier ID.")
    
    input("\nPress Enter to return to main menu...")


def delete_supplier_menu(system):
    clear_screen()
    print("            DELETE SUPPLIER")
    print("----------------------------------------")
    print()
    
    try:
        supplier_id = input("Enter Supplier ID to delete: ")
        print()
        system.delete_supplier(supplier_id)
    except ValueError:
        print("\nInvalid input! Please enter a numeric Supplier ID.")
    
    input("\nPress Enter to return to main menu...")


def update_supplier_menu(system):
    clear_screen()
    print("            UPDATE SUPPLIER")
    print("----------------------------------------")
    print()
    
    try:
        supplier_id = input("Enter Supplier ID to update: ")
        print()
        system.update_supplier(supplier_id)
    except ValueError:
        print("\nInvalid input! Please enter a numeric Supplier ID.")
    
    input("\nPress Enter to return to main menu...")


def display_suppliers_menu(system):
    clear_screen()
    system.display_allsuppliers()
    input("\nPress Enter to return to main menu...")


# Main menu interface
def main_menu():
    system = SupplierVendorManagementSystem()

    # Add sample data silently during initialization
    original_add_supplier = system.add_supplier
    
    def silent_add_supplier(supplier_id, supplier_name, supplier_contact, supplier_product, supplier_rating):
        new_supplier = Supplier(supplier_id, supplier_name, supplier_contact, supplier_product, supplier_rating)

        if system.root is None:
            system.root = BSTNode(new_supplier)
        else:
            system.root.insert(new_supplier)

    # Temporarily replace to load sample data without output
    system.add_supplier = silent_add_supplier
    create_sampledata(system)
    system.add_supplier = original_add_supplier

    while True:
        clear_screen()
        # Display main menu options
        print("PHARMACY SUPPLIER & VENDOR MANAGEMENT SYSTEM")
        print("---")
        print("1. Add New Supplier/Vendor")
        print("2. Search Supplier/Vendor by ID")
        print("3. Delete Supplier/Vendor by ID")
        print("4. Update Supplier/Vendor Information")
        print("5. Display All Suppliers/Vendors")
        print("6. Exit System")
        print("---")

        try:
            choice = input("Enter your choice (1-6): ").strip()

            if choice == '1':
                add_supplier_menu(system)
            elif choice == '2':
                search_supplier_menu(system)
            elif choice == '3':
                delete_supplier_menu(system)
            elif choice == '4':
                update_supplier_menu(system)
            elif choice == '5':
                display_suppliers_menu(system)
            elif choice == '6':
                clear_screen()
                print("Thank you for using Supplier & Vendor Management System!")
                print("Goodbye!")
                break
            else:
                print("Invalid choice! Please enter 1-6.")
                input("\nPress Enter to continue...")

        except ValueError:
            print("Invalid input! Please enter a numeric value where required.")
            input("\nPress Enter to continue...")
        except Exception as e:
            print(f"An error occurred: {e}")
            input("\nPress Enter to continue...")


if __name__ == "__main__":
    main_menu()
