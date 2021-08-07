package domain

type Order struct {
	Id 			int
	FirstAl		float64
	FirstLn		float64
	LastAl		float64
	LastLn		float64
}

type Hub struct {
	Id 			int
	Altitude 	float64
	Longitude 	float64
	TypeHub		int
}