package service

import (
	"io/ioutil"
	"math"
	"orderManager/internal/domain"
	"strconv"
	"strings"
)


func getHubs() (hubs[]domain.Hub, err error){
	data, err := ioutil.ReadFile("configs/validated_hubs.txt")
	if err != nil {
		return nil, err
	}

	dataLines := strings.Split(string(data), "\n")

	for i := 0; i < len(dataLines); i++ {

		if dataLines[i] != "" {

			dataLines := strings.Split(string(dataLines[i]), "   ")

			al, _ := strconv.ParseFloat(dataLines[2], 64)
			ln, _ := strconv.ParseFloat(dataLines[1], 64)
			t,  _ := strconv.Atoi(dataLines[0])

			newHub := domain.Hub{Id: i, Altitude: al, Longitude: ln, TypeHub: t}
			hubs = append(hubs, newHub)
		}
	}
	return hubs, nil
}

func getR(al1, al2, ln1, ln2 float64) float64{
	dln := ln2 - ln1
	ch := math.Pow(math.Cos(al2)*math.Sin(dln), 2) +
		math.Pow(math.Cos(al1)*math.Sin(al2)-math.Sin(al1)*math.Cos(al2)*math.Cos(dln), 2)
	zn := math.Sin(al1)*math.Sin(al2) + math.Cos(al1) * math.Cos(al2)*math.Cos(dln)

	return math.Atan2(ch, zn) * 6372795
}