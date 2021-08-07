package service

import (
	"errors"
	"fmt"
	"math"
	"orderManager/internal/domain"
	"orderManager/internal/repository"
	"os"
)

type Services struct {
	rep *repository.Repository
	Hubs[]domain.Hub
}

func (s *Services) GetNearHub(alPos, lnPos float64) (int, error){
	idHub := -1
	minL := math.Inf(1)
	for i := 0; i < len(s.Hubs); i++ {
		dR := getR(alPos, s.Hubs[i].Altitude, lnPos, s.Hubs[i].Longitude)
		if dR < minL{
			minL 	= dR
			idHub 	= s.Hubs[i].Id
		}
	}
	if idHub == -1 {
		return 0, errors.New("not found")
	}else{
		return idHub, nil
	}
}

func (s *Services) CreateNewOrder(order domain.Order) error{
	err := s.rep.CreateNewOrder(order)
	return err
}

func NewService(r *repository.Repository) *Services{
	hubs, err := getHubs()
	if err != nil{
		fmt.Println(err)
		os.Exit(1)
	}
	return &Services{r, hubs}
}

