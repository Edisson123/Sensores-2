using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Sprites;
using UnityEngine.UI;

public class boton_puerto : MonoBehaviour
{
    // Start is called before the first frame update
    Image imagen;
    void Start()
    {
        imagen = this.GetComponent<Image>();
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public void cambioColor()
    {
        imagen.color = Color.green;
    }
}
