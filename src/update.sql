declare
  h HISTO_TYPE;
  m FLOAT;
  n NUMERIC;
  t TAUX_TYPE;
begin

  -- chargement du contenu de l'image a partir du fichier
  select HISTOGRAMME, MOYENNE_NORME_GRADIENT, NB_PIXEL_COUNTOUR, TAUX_COULEURS into h, m, n, t
    from multimedia
    where nom = '1.jpg'
  for update;
  h := histo_type();
  m := 3.4;
  n := 250;
  t := taux_type(0.2, 0.4, 0.4);
  update multimedia
    set HISTOGRAMME = h, MOYENNE_NORME_GRADIENT = m, NB_PIXEL_COUNTOUR = n, TAUX_COULEURS = t
    where nom = '1.jpg';
  commit;
  
end;