#!/bin/bash

# Agregar todos los archivos modificados al área de preparación
git add .

# Confirmar los cambios con un mensaje descriptivo
read -p "Ingrese el mensaje de confirmación: " commit_message
git commit -m "$commit_message"

# Subir los cambios al repositorio de GitHub
git push origin main  # Ajusta 'main' al nombre de tu rama si es diferente

# Mostrar mensaje de éxito
echo "Cambios guardados en GitHub correctamente."