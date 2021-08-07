package handler

import (
	"fmt"
	"github.com/gin-gonic/gin"
)

func (h *Handler) CreateNewOrder(c *gin.Context){

	al1, al2, ln1, ln2:= 0.,0.,0.,0.

	fHub, _ := h.srv.GetNearHub(al1,ln1)
	lHub, _ := h.srv.GetNearHub(al2,ln2)

	_,_ = fHub, lHub

	fmt.Print("ordeeeeeeeeeeeeeeeer")
}






