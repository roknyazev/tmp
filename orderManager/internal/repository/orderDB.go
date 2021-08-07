package repository

import (
	"github.com/jmoiron/sqlx"
	"orderManager/internal/domain"
)

type Repository struct {
	db *sqlx.DB
}

func NewRepository(db *sqlx.DB) *Repository {
	return &Repository{db}
}

func (p *Repository) CreateNewOrder(o domain.Order) error {
	//var user domain.User
	//q := fmt.Sprintf("SELECT id FROM users WHERE username=$1 AND password=$2")
	//err := r.db.Get(&user, q, username, password)
	return nil
}


