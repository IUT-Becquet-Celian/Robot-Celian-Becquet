   M S B A M L   `   `   `  ���� W  SWpfSimplifiedWorldMapDisplay, Version=1.0.0.0, Culture=neutral, PublicKeyToken=nullO,clr-namespace:WpfSimplifiedWorldMapDisplayNSWpfSimplifiedWorldMapDisplayNS  A    ;WpfSimplifiedWorldMapDisplayNS.WpfSimplifiedWorldMapDisplay   W SPresentationCore, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35R NWindowsBase, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35\ XPresentationFramework, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35D 9http://schemas.microsoft.com/winfx/2006/xaml/presentation    R NSystem.Xaml, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089:x,http://schemas.microsoft.com/winfx/2006/xaml     Jmc;http://schemas.openxmlformats.org/markup-compatibility/2006     @d2http://schemas.microsoft.com/expression/blend/2008     >local,clr-namespace:WpfSimplifiedWorldMapDisplayNS     [ WSciChart.Drawing, Version=6.2.1.13304, Culture=neutral, PublicKeyToken=b55dd9efe817e823\ XSciChart.Charting, Version=6.2.1.13304, Culture=neutral, PublicKeyToken=b55dd9efe817e823X TSciChart.Core, Version=6.2.1.13304, Culture=neutral, PublicKeyToken=b55dd9efe817e823X TSciChart.Data, Version=6.2.1.13304, Culture=neutral, PublicKeyToken=b55dd9efe817e8237s)http://schemas.abtsoftware.co.uk/scichart     .��� .+�/  )SciChart.Charting.Visuals.SciChartSurface  -      Name$  sciChartSurface��3  -SciChart.Charting.Visuals.SciChartSurfaceBase   MaxFrameRate$ 25X�$��0q�$��0q�$��0��   XAxis 0  *SciChart.Charting.Visuals.Axes.NumericAxis  -  'SciChart.Charting.Visuals.Axes.AxisCore   VisibleRangeK  ESciChart.Charting.Common.Databinding.StringToDoubleRangeTypeConverter$ -12,12 -  'SciChart.Charting.Visuals.Axes.AxisBase   IsCenterAxis .     	AutoRange$ Never=�$y�Hidden=�   DrawMajorGridLines .     DrawMinorGridLines .     DrawMajorTicks .  	   DrawMinorTicks	 .  
   DrawMajorBands
 .     YAxis   $
 -8,8  .  $ Never=�$y�Hidden=� .   .   .  	 .  
 .     RenderableSeries <   6WpfSimplifiedWorldMapDisplayNS.PolygonRenderableSeries  -     Name$ PolygonTerrainSeries�� J� Opacity$	 0.5Q�J  DSciChart.Charting.Visuals.RenderableSeries.XyScatterRenderableSeries  -     Name$ LidarPoints��$	 0.5Q�E	  ?SciChart.Charting.Visuals.RenderableSeries.BaseRenderableSeries 	  PointMarker ?
  9SciChart.Charting.Visuals.PointMarkers.EllipsePointMarker
  $��2��$��2��<  6SciChart.Charting.Visuals.PointMarkers.BasePointMarker   Fill$ 	SteelBlue��   Stroke$ LightSteelBlue��   StrokeThickness$ 1Q�  -   $ LidarProcessedPoints��$	 0.5Q� 
  $��2��$��2��$ 	SteelBlue��$ 
LightCoral��$ 1Q�  -   $ PolygonSeries��$	 0.5Q�  -   $ 
BallPoints��$	 0.5Q� 
  $��10��$��10��$ Yellow��$ White��$ 1Q�  -   $ BallPolygon��$	 0.8Q�  -   $ ObjectsPolygonSeries��$	 0.8Q�  -	   $ ObstaclePoints��$	 0.5Q� 
  $��5��$��5��$	 Red��$ White��$ 1Q�   Annotations :  4SciChart.Charting.Visuals.Annotations.TextAnnotation  -
     Name$ LocalWorldMapTitle��:  4SciChart.Charting.Visuals.Annotations.AnnotationBase   AnnotationCanvas$ 
AboveChart=�   CoordinateMode$ Relative=�$��14(�$
��Bold"�A  ;SciChart.Charting.Visuals.Annotations.AnchorPointAnnotation   HorizontalAnchorPoint$
 Left=�$	 0.5Q�   Text$ Local World Map��   VerticalAnchorPoint$ Center=�	   X1	  dbs$
 0.15 	   Y1$
 0.18   -   $ AnnotRobotRole��$ 
AboveChart=�$ Relative=�$��14(�$
��Bold"�$ Right=�$	 0.5Q�$
 Role��$ Center=�$	 0.8 $
 0.18 9  3SciChart.Charting.Visuals.Annotations.BoxAnnotation  -     Name$ AnnotationFond��$ 
BelowChart=�$ Absolute=�$��Stretch=����	#55FFFF00 �� *X1+	   X2 �� *X2+ �� *Y1+	   Y2 �� *Y2+����   �� 
TargetType"   .R��� ! ��Name$! PART_BoxAnnotationRoot��._��� $��BackGroundImage��$
��Fill=�$	 0.2Q����� *	ImagePath+"   ChartModifier" 4  .SciChart.Charting.ChartModifiers.ModifierGroup  #   ChildModifiers.# ?  9SciChart.Charting.ChartModifiers.RubberBandXyZoomModifier  $   IsXAxisOnly$ .  %   ZoomExtentsY% .  &   
IsAnimated& . '   RubberBandFill' �	#33FFFFFF(   RubberBandStroke( �	#66FFFFFF )   RubberBandStrokeDashArray$	) 2 2R�=  7SciChart.Charting.ChartModifiers.MouseWheelZoomModifier  *   
ActionType$
* Zoom=�?  9SciChart.Charting.ChartModifiers.RelativeZoomModifierBase+   XyDirection$+ XYDirection=�