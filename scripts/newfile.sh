#!/bin/bash

# == CONFIGURACIÓN GENERAL ==
AUTHOR="Oscar Sotomayor"
LICENSE_URL="https://mozilla.org/MPL/2.0/"
YEAR=$(date +%Y)

# == INPUT DEL USUARIO ==
read -p "Nombre base del archivo (sin extensión): " BASENAME
read -p "Tipo de archivo (header/source/example): " TYPE

# == EXTENSIÓN Y ARCHIVO ==
EXT=""
case "$TYPE" in
    header) EXT="h" ;;
    source) EXT="c" ;;
    example) EXT="c" ;;
    *) echo "Tipo inválido. Usa: header, source o example." ; exit 1 ;;
esac

FILENAME="${BASENAME}.${EXT}"
if [ -e "$FILENAME" ]; then
    echo "❌ El archivo '$FILENAME' ya existe. Cancelado."
    exit 1
fi

# == GENERADOR DE ENCABEZADO ==
HEADER=""

if [ "$TYPE" == "header" ] || [ "$TYPE" == "source" ]; then
    HEADER="/*
 * This file is part of the NeuroTIC project.
 * Author: $AUTHOR
 * License: Mozilla Public License v. 2.0 ($LICENSE_URL)
 * Copyright (c) $YEAR $AUTHOR
 */
"
elif [ "$TYPE" == "example" ]; then
    HEADER="/*
 * Example usage of NeuroTIC - A neural network library by $AUTHOR.
 * This example is provided under the terms of the Mozilla Public License v. 2.0.
 * See: $LICENSE_URL
 */
"
fi

# == ESCRITURA DEL ARCHIVO ==
echo "$HEADER" > "$FILENAME"
echo "// File: $FILENAME" >> "$FILENAME"
echo -e "\n✅ Archivo creado: $FILENAME"
