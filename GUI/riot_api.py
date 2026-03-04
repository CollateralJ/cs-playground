# My first python project. Uses a simple ctk gui to pull data from the riot games API.

import customtkinter as tk
import webbrowser
import requests
import json
from tkinter import messagebox as msg

RIOT_API_LINK = "https://developer.riotgames.com/"

def browser_open_link(link: str):
    webbrowser.open(link)

def send_msg(print_message: str):
    msg.showinfo("API Test Message", print_message)

def on_API_pull_button_pressed():
    # get riot puuid
    gameName = str(user_entry.get())
    tagLine = str(tag_entry.get())
    RIOT_API_KEY = str(key_entry.get())
    RIOT_HEADERS = {"X-Riot-Token": RIOT_API_KEY}
    r = requests.get(f"https://americas.api.riotgames.com/riot/account/v1/accounts/by-riot-id/{gameName}/{tagLine}", headers=RIOT_HEADERS)
    puuid = ""
    print(f"puuid get status: {r.status_code}")
    try:
        puuid = r.json()["puuid"]
        print(f"puuid found: {puuid}")
    except:
        print("no puuid found")

    r = requests.get(f"https://na1.api.riotgames.com/lol/league/v4/entries/by-puuid/{puuid}", headers=RIOT_HEADERS)
    status_label.configure(text=f"Executed with return code: {r.status_code}")
    print(f"player data get status: {r.status_code}")
    textbox.delete("1.0", "end")
    try:
        textbox.insert("end", json.dumps(r.json()[0]))
    except:
        print("no player data found")

def on_API_link_button_pressed():
    browser_open_link(RIOT_API_LINK)

tk.set_appearance_mode("dark")
tk.set_default_color_theme("blue")

root = tk.CTk()
root.title("API Test")
root.geometry("600x400")

label = tk.CTkLabel(root, text="Welcome to Riot API Tester Program")
label.grid(row=0, column=0)

button = tk.CTkButton(root, text="Press this button to test API.", command = on_API_pull_button_pressed)
button.grid(row=0, column=1)

key_label = tk.CTkLabel(root, text="Riot Developer API Key")
key_label.grid(row=1, column=0)

key_entry = tk.CTkEntry(root)
key_entry.grid(row=1, column=1)

user_label = tk.CTkLabel(root, text="Riot Username")
user_label.grid(row=2, column=0)

user_entry = tk.CTkEntry(root)
user_entry.grid(row=2, column=1)

tag_label = tk.CTkLabel(root, text="Tagline")
tag_label.grid(row=3, column=0)

tag_entry = tk.CTkEntry(root)
tag_entry.grid(row=3, column=1)

status_label = tk.CTkLabel(root, text="")
status_label.grid(row=4, column=0)

textbox = tk.CTkTextbox(root)
textbox.grid(row=4, column=1)

label = tk.CTkLabel(root, text="Don't have your riot API key?")
label.grid(row=5, column=0)

button = tk.CTkButton(root, text="Get my key (opens browser)", command = on_API_link_button_pressed)
button.grid(row=5, column=1)

root.mainloop()




