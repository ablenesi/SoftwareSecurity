from tkinter import *
from os import system
command='rlogin -l '
def rlogin():
    system(command+entryWidget.get().strip()+' '+entry.get().strip())
root = Tk()
root.title("Rlogin connect")
root["padx"] = 40
root["pady"] = 20   
# Create a text frame to hold the text Label and the Entry widget
textFrame = Frame(root)
#Create a Label in textFrame
user = Label(textFrame)
user["text"] = "User:"
user.pack(side=LEFT)
# Create an Entry Widget in textFrame
entryWidget = Entry(textFrame)
entryWidget["width"] = 25
entryWidget.pack(side=LEFT)
host = Label(textFrame)
host["text"] = "Host:"
host.pack(side=LEFT)
entry = Entry(textFrame)
entry["width"] = 25
entry.pack(side=LEFT)
textFrame.pack()
button = Button(root, text="Login", command=rlogin)
exitBtn = Button(root, text="Exit", command=exit)
exitBtn.pack(side=LEFT)
button.pack(side=LEFT)
root.mainloop()
