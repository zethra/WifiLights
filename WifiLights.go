package main
import (
	"net/http"
	"github.com/gorilla/websocket"
	"log"
)

type Status struct {
	connected bool
	message chan string
}

var status = Status{
	connected: false,
	message: make(chan string),
}

var upgrader = websocket.Upgrader{
	ReadBufferSize:  1024,
	WriteBufferSize: 1024,
}


func main() {
	http.Handle("/", http.FileServer(http.Dir("web")))
	http.HandleFunc("/ctrl", controlHandler)
	http.HandleFunc("/io", socketHandler)
	http.ListenAndServe(":8080", nil)
}

func controlHandler(writer http.ResponseWriter, request *http.Request) {
	err := request.ParseForm()
	if (err != nil) {
		http.Error(writer, "Cannot parse form", 500)
		return
	}
	message := request.PostFormValue("message")
	log.Println("Recieved message: " + message)
	status.message <- message
}

func socketHandler(writer http.ResponseWriter, request *http.Request) {
	conn, err := upgrader.Upgrade(writer, request, nil)
	if err != nil {
		log.Println(err)
		return
	}
	messageType, p, err := conn.ReadMessage()
	if err != nil {
		log.Println(err)
		return
	}
	log.Println("Recieved: ", string(p))
	if(string(p) != "start") {
		return
	}
	log.Println("Waiting for command")
	for {
		command := <- status.message
		if err = conn.WriteMessage(messageType, []byte(command)); err != nil {
			log.Println(err)
			return
		}
		log.Println("Sent command: " + command)
	}
}