package main

import (
	"fmt"
	"orderManager/internal/config"
	"orderManager/internal/handler"
	"orderManager/internal/repository"
	"orderManager/internal/server"
	"orderManager/internal/service"
	"os"
)

func main(){

	Config, err := config.Init("configs")
	if err != nil {
		fmt.Println(err)
		os.Exit(1)
	}

	db, err 	:= repository.NewPostgresDB(Config)
	if err != nil {
		fmt.Println(err)
		os.Exit(1)
	}

	rep     	:= repository.NewRepository(db)
	sc		 	:= service.NewService(rep)
	Handlers 	:= handler.NewHandler(sc)
	srv 		:= server.NewServer(Config, Handlers.InitRoutes())
	err = srv.Run()
	if err != nil {
		return
	}
}


