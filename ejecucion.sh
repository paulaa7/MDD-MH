#!/bin/bash

EXE="main"
PARAM=15
FITNESS_ONLY="solo_fitness.csv"
TIEMPO_ONLY="solo_tiempo.csv"

# Borra archivos anteriores si existen
if [ -f "$FITNESS_ONLY" ]; then
    rm "$FITNESS_ONLY"
fi

if [ -f "$TIEMPO_ONLY" ]; then
    rm "$TIEMPO_ONLY"
fi

# Lista de casos
LIST=("GKD-b_1_n25_m2" "GKD-b_2_n25_m2" "GKD-b_3_n25_m2" "GKD-b_4_n25_m2" "GKD-b_5_n25_m2" "GKD-b_6_n25_m7" "GKD-b_7_n25_m7" "GKD-b_8_n25_m7" "GKD-b_9_n25_m7" "GKD-b_10_n25_m7" "GKD-b_11_n50_m5" "GKD-b_12_n50_m5" "GKD-b_13_n50_m5" "GKD-b_14_n50_m5" "GKD-b_15_n50_m5" "GKD-b_16_n50_m15" "GKD-b_17_n50_m15" "GKD-b_18_n50_m15" "GKD-b_19_n50_m15" "GKD-b_20_n50_m15" "GKD-b_21_n100_m10" "GKD-b_22_n100_m10" "GKD-b_23_n100_m10" "GKD-b_24_n100_m10" "GKD-b_25_n100_m10" "GKD-b_26_n100_m30" "GKD-b_27_n100_m30" "GKD-b_28_n100_m30" "GKD-b_29_n100_m30" "GKD-b_30_n100_m30" "GKD-b_31_n125_m12" "GKD-b_32_n125_m12" "GKD-b_33_n125_m12" "GKD-b_34_n125_m12" "GKD-b_35_n125_m12" "GKD-b_36_n125_m37" "GKD-b_37_n125_m37" "GKD-b_38_n125_m37" "GKD-b_39_n125_m37" "GKD-b_40_n125_m37" "GKD-b_41_n150_m15" "GKD-b_42_n150_m15" "GKD-b_43_n150_m15" "GKD-b_44_n150_m15" "GKD-b_45_n150_m15" "GKD-b_46_n150_m45" "GKD-b_47_n150_m45" "GKD-b_48_n150_m45" "GKD-b_49_n150_m45" "GKD-b_50_n150_m45")

TOTAL=${#LIST[@]}
COUNT=0

for CASE in "${LIST[@]}"; do
    fitness=""
    tiempo=""
   
    ((COUNT++))
    echo "Procesando caso $COUNT de $TOTAL: $CASE"
   
    # Ejecutar el comando con el nombre del caso como parámetro
    while IFS= read -r line; do
        # Buscar y capturar la línea de "Fitness:"
        echo "$line" | grep -q "Fitness:"
        if [ $? -eq 0 ]; then
            fitness=$(echo "$line" | awk -F":" '{print $2}' | tr -d '[:space:]')
            fitness=$(echo "$fitness" | sed 's/\./,/g')  # Reemplaza el punto por coma
        fi

        # Buscar y capturar la línea de "Time:"
        echo "$line" | grep -q "Time:"
        if [ $? -eq 0 ]; then
            tiempo=$(echo "$line" | awk -F":" '{print $2}' | tr -d '[:space:]')
            tiempo=$(echo "$tiempo" | sed 's/\./,/g')  # Reemplaza el punto por coma
        fi
    done < <(./$EXE $PARAM "$CASE")
   
    # Si el fitness fue encontrado, lo guarda
    if [ -n "$fitness" ]; then
        echo "$fitness" >> "$FITNESS_ONLY"
    else
        echo "N/A" >> "$FITNESS_ONLY"
    fi

    # Si el tiempo fue encontrado, lo guarda
    if [ -n "$tiempo" ]; then
        echo "$tiempo" >> "$TIEMPO_ONLY"
    else
        echo "N/A" >> "$TIEMPO_ONLY"
    fi
done

echo
echo "Proceso completado."
echo "Fitness guardado en: $FITNESS_ONLY"
echo "Tiempo guardado en:  $TIEMPO_ONLY"