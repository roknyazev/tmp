package handler

import (
	"github.com/gin-contrib/cors"
	"github.com/gin-gonic/gin"
	"orderManager/internal/service"
)

type Handler struct {
	srv *service.Services
}

func NewHandler(s *service.Services) *Handler {
	return &Handler{s}
}

func (h *Handler) InitRoutes() *gin.Engine {
	router := gin.New()
	router.Use(cors.Default())

	router.NoRoute(func(c *gin.Context) {
		c.JSON(404, gin.H{"code": "PAGE_NOT_FOUND", "message": "хуй соси"})
	})

	router.POST("/order", h.CreateNewOrder)

	return router
}




