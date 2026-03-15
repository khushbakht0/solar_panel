# Solar Panel Profile Management System

A C++ console application that helps Pakistani homeowners calculate 
their solar panel requirements, compare panel types, and manage 
personalized energy profiles — all in one place.

> Built as a portfolio project -> 1st Semester -> FAST-NU

---

## 📌 Overview

With rising electricity costs in Pakistan, this system helps 
homeowners make informed decisions about switching to solar energy. 
Users can create profiles, input their home details and appliances, 
and instantly get recommendations on how many solar panels they need 
and how much it will cost.

---

## ✨ Features

### 👤 Profile Management
- Create personalized solar energy profiles
- Auto-generated unique 5-digit password per profile
- View, update, and delete profiles securely
- Password verification required before deletion

### 🏠 Smart Home Analysis
- Supports house sizes in **Marla, Kanal, and Square Feet**
- Calculates power consumption based on rooms + appliances
- Handles up to 100 appliances per profile
- Tracks appliance name, quantity, and wattage

### ⚡ Solar Panel Recommendation Engine
- Choose from 3 real panel types available in Pakistan:
  | Panel Type | Output | Efficiency | Price (PKR) |
  |---|---|---|---|
  | N-Type Bifacial | 585W | 22.67% | Rs. 18,135 |
  | A-Grade | 550W | 19.5% | Rs. 13,570 |
  | Mono Facial | 560W | 21% | Rs. 15,540 |
- Calculates exact number of panels needed
- Estimates total installation cost including inverter costs

### 💾 File Handling (Data Persistence)
- All profiles saved to `profiles.txt` automatically
- Profiles reload on every startup — no data loss
- Full CRUD operations on saved data

### 🛡️ Input Validation
- Rejects negative numbers and non-integer inputs
- Handles invalid unit entries with re-prompt
- Protects against cin failure and buffer overflow

---

## 🚀 How to Run

### Requirements
- C++ compiler (g++ / MinGW)
- Windows / Linux / Mac

### Steps
```bash
# Compile
g++ solar_panel.cpp -o solar_panel

# Run
./solar_panel
```

---

## 📸 Demo

*Run the program and:*
1. Select **1** to create a profile
2. Enter your house size (e.g. `5 marla`)
3. Add your appliances and wattage
4. Choose your preferred panel type
5. Get instant cost and panel recommendations!

---

## 🧠 Concepts Used

- Structs & Arrays
- File I/O (fstream, ofstream, ifstream)
- String manipulation & parsing
- Functions & recursion
- Input validation & error handling
- Math functions (ceil)
- Output formatting (iomanip, setw)

---

## 👩‍💻 Author

**Khushbakht** — 1st Semester CS Student @ FAST-NU  
[GitHub](https://github.com/khushbakht0)

---

## 📝 Note

This project was built entirely in C++ without any external 
libraries, demonstrating core programming fundamentals learned 
in the first semester
