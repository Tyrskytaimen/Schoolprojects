"""
COMP.CS.100 Programming 1
Student ID: H299784                     050250033
Name:       Katariina Tenhunen          Miika Isberg
Email:      katariina.tenhunen@tuni.fi  miika.isberg@tuni.fi

Tämä työ on kurssilla aiemmin olleesta autotehtävästä luotu
graafinen käyttöliittymä. Auto luodaan Car-luokassa ja itse
käyttöliittymä toimii Carwindow-luokassa.

Käyttäjä antaa autolle ensin nimen, tankin tilavuuden ja kulutuksen.
Käyttäjä voi tankata auto tai ajaa sen tiettyihin koordinaatteihin.
Käyttäjällä on myös mahdollisuus aloittaa ajaminen samalla autolla
alusta tai luoda täysin uusi auto.

Käyttäjä ei voi aloittaa alusta, tai luoda uutta autoa, ennen kuin
auton tilaan on syntynyt jokin muutos (auto tankattu). Autoa ei
myöskään ole mahdollista ajaa tyhjällä tankilla, eikä täyteen tankkiin
voi tankata enempää.

Työ tähtää kehittyneeseen versioon projektista.
"""
from tkinter import *
from auto import *
from random import choice
import webbrowser


URL = "https://urly.fi/NM3"
FONT = ("Comic Sans MS", 9, "bold")


class Carwindow:
    def __init__(self):
        # Valitaan random väri ohjelman taustalle
        colorlist = ["#b4cdd4", "#f0ccd5", "#edebc0", "#c2edc0", "#d6c8e3", "#f0ac75"]
        bgcolor = choice(colorlist)

        self.mainwindow = Tk()
        self.mainwindow.title("Car")
        self.mainwindow.iconbitmap("kuvat/autokuvake.ico")
        self.mainwindow.configure(bg=bgcolor)

        # Menun luonti
        self.menubar = Menu(self.mainwindow)
        self.dropmenu = Menu(self.menubar, tearoff=0, bg="white", font=FONT)
        self.dropmenu.add_command(label="Start again", state=DISABLED, command=self.startagain)
        self.dropmenu.add_command(label="New car", state=DISABLED, command=self.newcar)
        self.dropmenu.add_separator()
        self.dropmenu.add_command(label="Quit", font=FONT, command=self.quit)
        self.menubar.add_cascade(label="Menu", menu=self.dropmenu)
        self.menubar.add_command(label="?", command=open_url)
        self.mainwindow.config(menu=self.menubar)

        # Auton luontiin tarvittavat widgetit
        self.name = Entry(relief=FLAT)
        self.tanksize = Entry(relief=FLAT)
        self.consumption = Entry(relief=FLAT)
        self.namelabel = Label(self.mainwindow, text="Car's name:", bg=bgcolor, font=FONT)
        self.tanklabel = Label(self.mainwindow, text="Tank size (L):", bg=bgcolor, font=FONT)
        self.consumptionlabel = Label(self.mainwindow, text="Consumption (L/100km):", bg=bgcolor, font=FONT)
        self.createbutton = Button(self.mainwindow, text="Create", activebackground="white", font=FONT,
                                   command=self.create)
        self.explanation = Label(self.mainwindow, bg=bgcolor, text="Create a car!", font=FONT)

        self.explanation.grid(row=0, column=0, columnspan=6)
        self.namelabel.grid(row=1, column=0, sticky="E")
        self.name.grid(row=1, column=1, columnspan=3, sticky="WE")
        self.tanklabel.grid(row=2, column=0, sticky="E")
        self.tanksize.grid(row=2, column=1, columnspan=3, sticky="WE")
        self.consumptionlabel.grid(row=3, column=0, sticky="E")
        self.consumption.grid(row=3, column=1, columnspan=3, sticky="WE")
        self.createbutton.grid(row=4, column=1, columnspan=3, pady=5, sticky="WE")

        # Kuvien tallentaminen muutujiin
        self.carphoto = PhotoImage(file="kuvat/autonkuva.png")
        self.drivephoto = PhotoImage(file="kuvat/drive.png")
        self.vroomphoto = PhotoImage(file="kuvat/vroom.png")
        self.emptyphoto = PhotoImage(file="kuvat/0.png")
        self.quarterphoto = PhotoImage(file="kuvat/25.png")
        self.halfphoto = PhotoImage(file="kuvat/50.png")
        self.almostfullphoto = PhotoImage(file="kuvat/75.png")
        self.fullphoto = PhotoImage(file="kuvat/100.png")

        self.carlabel = Label(self.mainwindow, image=self.carphoto, bg=bgcolor)
        self.vroomlabel = Label(self.mainwindow, image=self.vroomphoto, bg=bgcolor)
        self.emptyrow1 = Label(self.mainwindow, bg=bgcolor)

        self.carlabel.grid(row=6, column=0, rowspan=6, sticky="S")
        self.vroomlabel.grid(row=1, column=5, rowspan=16, sticky="NS")
        self.emptyrow1.grid(row=5, column=0)

        # Auton tietoja varten tarvittavat widgetit
        self.carname = Label(self.mainwindow, text="Name: ", bg=bgcolor, font=FONT)
        self.in_tank = Label(self.mainwindow, text="Gas: ", bg=bgcolor, font=FONT)
        self.odometer = Label(self.mainwindow, text="Kilometers: ", bg=bgcolor, font=FONT)
        self.x = Label(self.mainwindow, text="x: ", bg=bgcolor, font=FONT)
        self.y = Label(self.mainwindow, text="y: ", bg=bgcolor, font=FONT)

        self.carname.grid(row=7, column=1, sticky="WS", columnspan=2)
        self.in_tank.grid(row=8, column=1, sticky="W", columnspan=2)
        self.odometer.grid(row=9, column=1, sticky="WN", columnspan=2)
        self.x.grid(row=10, column=1, sticky="WN")
        self.y.grid(row=11, column=1, sticky="WN")

        # Ajoa ja tankkausta varten widgetit
        self.emptyrow2 = Label(self.mainwindow, bg=bgcolor)
        self.xcoord = Entry(width=4, relief=FLAT)
        self.ycoord = Entry(width=4, relief=FLAT)
        self.liters = Entry(width=4, relief=FLAT)
        self.xlabel = Label(self.mainwindow, text="x:", bg=bgcolor, font=FONT)
        self.ylabel = Label(self.mainwindow, text="y:", bg=bgcolor, font=FONT)
        self.coordlabel = Label(self.mainwindow, text="Coordinates:", bg=bgcolor, font=FONT)
        self.literslabel = Label(self.mainwindow, text="Liters:", bg=bgcolor, font=FONT)
        self.drivebutton = Button(self.mainwindow, image=self.drivephoto, state=DISABLED,
                                  activebackground="white", command=self.drive)
        self.fillbutton = Button(self.mainwindow, image=self.emptyphoto, state=DISABLED,
                                 activebackground="white", command=self.fill)

        self.emptyrow2.grid(row=12, column=0)
        self.coordlabel.grid(row=13, column=1, columnspan=3, sticky="WS")
        self.xlabel.grid(row=14, column=1, sticky="W")
        self.xcoord.grid(row=14, column=2, sticky="W")
        self.ylabel.grid(row=15, column=1, sticky="WN")
        self.ycoord.grid(row=15, column=2, sticky="WN")
        self.drivebutton.grid(row=13, column=0, rowspan=3, pady=5, sticky="NESW")
        self.literslabel.grid(row=16, column=1, sticky="W")
        self.liters.grid(row=16, column=2, sticky="W")
        self.fillbutton.grid(row=16, column=0, sticky="NESW")
        
        self.car = NONE

    def newcar(self):
        """
        Aloittaa ohjelman alusta, jolloin käyttäjä voi syöttää autolle
        uudet tiedot.
        """
        self.empty_information()
        self.createbutton.configure(state=NORMAL)
        self.fillbutton.configure(state=DISABLED)
        self.drivebutton.configure(state=DISABLED)
        self.explanation.configure(text="Create a car!", fg="black")
        self.dropmenu.entryconfigure(0, state=DISABLED)
        self.dropmenu.entryconfigure(1, state=DISABLED)

    def startagain(self):
        """
        Aloittaa ohjelman uudestaan, edellisen auton tiedoilla.
        """
        try:
            self.car.zeroing()
        except AttributeError:
            return

        self.empty_information()
        self.update()

        self.tanksize.insert(0, f"{self.car.get_tankvolume():.1f}")
        self.name.insert(0, f"{self.car.get_name()}")
        self.consumption.insert(0, f"{self.car.get_consumption():.1f}")

        self.fillbutton.configure(state=NORMAL)
        self.drivebutton.configure(state=DISABLED)
        self.explanation.configure(text=f"Give {self.car.get_name()} some gas!", fg="black")
        self.dropmenu.entryconfigure(0, state=DISABLED)

    def create(self):
        """
        Luo uuden Car-luokkaisen auton. Käsittelee myös luonnissa
        syntyvät mahdolliset virhetilanteet.
        """
        name = self.name.get().lstrip().rstrip()
        if name == "":
            self.explanation.configure(text="Your car needs a name!", fg="#b02525")
            self.empty_information()
            return

        try:
            tanksize = float(self.tanksize.get())
            consumption = float(self.consumption.get())
        except ValueError:
            self.explanation.configure(text=f"{name} is a great name, but the other two values must be numbers!",
                                       fg="#b02525")
            self.empty_information()
            self.name.insert(0, name)
            return

        if tanksize <= 0 or consumption <= 0:
            self.explanation.configure(text="Those can't be negative!", fg="#b02525")
            self.empty_information()
            return

        self.car = Car(name, tanksize, consumption)

        self.update()

        self.createbutton.configure(state=DISABLED)
        self.fillbutton.configure(state=NORMAL)
        self.explanation.configure(text=f"Give {self.car.get_name()} some gas!", fg="black")
        self.dropmenu.entryconfigure(1, state=NORMAL)

    def fill(self):
        """
        Tankkaa auton.
        """
        try:
            to_fill = float(self.liters.get())
        except ValueError:
            self.explanation.configure(text="Try giving liters as numbers!", fg="#b02525")
            self.liters.delete(0, END)
            return

        if to_fill <= 0:
            self.explanation.configure(text=f"You can't take gas away from {self.car.get_name()}!", fg="#b02525")
            self.liters.delete(0, END)
            return

        self.car.fill(to_fill)

        self.update()
        self.liters.delete(0, END)
        self.drivebutton.configure(state=NORMAL)
        self.dropmenu.entryconfigure(0, state=NORMAL)
        self.explanation.configure(text=f"Let's drive around with {self.car.get_name()}!", fg="black")

    def drive(self):
        """
        Ajaa auton annettuihin koordinaatteihin.
        """
        try:
            x = float(self.xcoord.get())
            y = float(self.ycoord.get())

        except ValueError:
            self.explanation.configure(text="Try giving coordinates as numbers!", fg="#b02525")
            self.xcoord.delete(0, END)
            self.ycoord.delete(0, END)
            return

        self.car.drive(x, y)

        self.update()
        self.fillbutton.configure(state=NORMAL)
        self.explanation.configure(text="Woooooooo!!", fg="black")

        self.xcoord.delete(0, END)
        self.ycoord.delete(0, END)

    def update(self):
        """
        Päivittää auton tiedot ja tankkauspainikkeen kuvakkeen.
        """
        ratio = (self.car.get_gas() / self.car.get_tankvolume()) * 100

        self.carname.configure(text=f"Name: {self.car.get_name()}")
        self.in_tank.configure(text=f"Gas: {self.car.get_gas():.1f} ({ratio:.0f}%)")
        self.odometer.configure(text=f"Kilometers: {self.car.get_odometer():.1f}")

        x, y = self.car.get_coordinates()
        self.x.configure(text=f"x: {x:.1f}")
        self.y.configure(text=f"y: {y:.1f}")

        if int(ratio) == 100:
            self.fillbutton.configure(state=DISABLED, image=self.fullphoto)
        elif int(ratio) in range(70, 100):
            self.fillbutton.configure(image=self.almostfullphoto)
        elif int(ratio) in range(40, 70):
            self.fillbutton.configure(image=self.halfphoto)
        elif int(ratio) in range(1, 40):
            self.fillbutton.configure(image=self.quarterphoto)
        else:
            self.fillbutton.configure(image=self.emptyphoto)
            self.drivebutton.configure(state=DISABLED)

    def empty_information(self):
        """
        Tyhjentää kaikki auton tiedot käyttöliittymästä.
        """
        self.tanksize.delete(0, END)
        self.name.delete(0, END)
        self.consumption.delete(0, END)
        self.carname.configure(text=f"Name:")
        self.in_tank.configure(text=f"Gas:")
        self.odometer.configure(text=f"Kilometers:")
        self.x.configure(text=f"x: ")
        self.y.configure(text=f"y: ")

    def quit(self):
        self.mainwindow.destroy()


def open_url():
    webbrowser.open_new(URL)


def main():
    game = Carwindow()
    game.mainwindow.mainloop()


if __name__ == "__main__":
    main()
